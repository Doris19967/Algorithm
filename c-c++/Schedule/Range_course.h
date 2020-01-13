struct CourseType {
	char course_no[5];        //�γ̱��
	char course_name[100];    //�γ���
    int period;               //ѧʱ��
    int term;				  //����ѧ��
};

bool operator ==(const CourseType &first, const CourseType &second)
{
   return strcmp(first.course_no, second.course_no)==0;
}


bool operator !=(const CourseType &first, const CourseType &second)
{
   return strcmp(first.course_no, second.course_no)!=0;
}


Extended_queue<int> &assign(Extended_queue<int> &q, const List<int> &l)
{   //�����Ա�ֵ������
	int position,x;
	q.clear();
	for(position=0;position<l.size();position++){
		l.retrieve(position,x);
		q.append(x);
	}
	return q;
}
 

List<int> &assign(List<int> &l, Extended_queue<int> q)
{   //�����и�ֵ�����Ա�
	int position=0,x;
	l.clear();
	while(q.empty()==false){
		q.retrieve(x);
		q.serve();
		l.insert(position++,x);
	}
	return l;
}


//terms_num��ʾҪ�ſε�ѧ������max_size��ʾ������ŵĿγ���
template<int terms_num,int max_size>      
class Range_Course{
public:
	Range_Course(ifstream *file_in, ofstream *file_out);
	void read();  //���������������й���Ϣ���Խ���һ������ͼ
	//����������ʽ�����ſ�
	void topological_order();
	void write(); //����α���Ϣ 
private:
	Digraph<CourseType,max_size> g;          //�ɿγ���Ϣ��������ͼ  
	ifstream *infile;                    //������ 
    ofstream *outfile;                   //�����
	int indegree[max_size];              //��������           
	Extended_queue<int> q[terms_num+1];	 //�����Ϊ0�Ľ�㻺�����
	//�α�,�±�ֱ��ʾ��ѧ�ڡ��ڴΡ����ڡ��γ���
	char course_table[terms_num+1][11][6][100];
    int course_num[terms_num+1];         //ÿѧ�ڵĿγ���
	int locate_vex(char course_no[]);    //���ض�λ����
	void readln();                       //����  
	char get_char();                     //��������������һ��������ַ�  
	bool range(int num,int term,char course_name[]);//�ſ�
	bool range_one(int &week_day,int term,char course_name[]);//��һ���1�ڿ�
	bool range_two(int &week_day,int term,char course_name[]);//��һ���2�ڿ�
	bool range_three(int &week_day,int term,char course_name[]);//��һ���3�ڿ�
	int find_one(int week_day,int term);  //�Ҳ�һ��1�ڿտ�
	int find_two(int week_day,int term);  //�Ҳ�һ������2�ڿտ�
	int find_three(int week_day,int term);//�Ҳ�һ������3�ڿտ�
	void write(char s[],int l);           //���ָ�����ȵ��ַ��� 
	char *decimal_to_chinese_char(int n,char *s); //�����ı�ʾʮ������
	void clear_course_table(int term);    //��յ�termѧ�ڵĿα�
	//�õݹ���ٷ�ʽ�ſ�
	bool recursive_topological_order(const List<int> &lq,int start,const List<int> &lt,int term,int ind[]);  

};


template<int terms_num,int max_size> 
Range_Course<terms_num,max_size>::Range_Course(ifstream *file_in, ofstream *file_out)
{
   infile = file_in;
   outfile = file_out;

   int i,j,k;
   for(i=1;i<=terms_num;i++)                     //ѧ����
	   for(j=1;j<=10;j++)                        //�ڴ�
		   for(k=1;k<=5;k++)                     //����   
			   strcpy(course_table[i][j][k],""); //��տα� 
   for(i=0;i<max_size;i++)indegree[i]=0;

}


template<int terms_num,int max_size> 
void Range_Course<terms_num,max_size>::read()
{   //���������������й���Ϣ���Խ���һ������ͼ
	char c;
	int i,n,v,w,course_total_num=0;
	CourseType course;
	char prio_course_no[5];  //�Ⱦ��γ̺�

	//����Ϊ���������������ѧ�������Ŀγ���
	(*infile).seekg(0);
	c = get_char();
	while(c<'0'||c>'9'){  //�������ֿ�ʼ����
		if(c!='\n')readln();
		c = get_char();
	}
	(*infile).putback(c);
	for(i=1;i<=terms_num;i++){
		(*infile)>>n;
		course_num[i]=n;
		course_total_num+=n;
	}

	//����Ϊ����������������γ���Ϣ(����ͼ�Ķ�����Ϣ)
	(*infile).seekg(0);
	c = get_char();
	while(c!=EOF){
		while(c!=EOF&&c!='c'){  //������'c'��ʼ����
			if(c!='\n')readln();
			c = get_char();
		}
		if(c!=EOF){             //������'c'��ʼ����
			(*infile).putback(c);
			(*infile)>>course.course_no;
			(*infile)>>course.course_name;
			(*infile)>>course.period;
			(*infile)>>course.term;
			readln();
			if(g.insert_vex(course)!=success){
				cout<<"���Ա����!"<<endl;
				exit(1);
			}
		}
		c = get_char();
	}

	//����Ϊ����������������γ��Ⱦ�������Ϣ(����ͼ�Ļ���Ϣ)
	(*infile).seekg(0);
	c = get_char();
	while(c!=EOF){
		while(c!=EOF&&c!='c'){  //������'c'��ʼ����
			if(c!='\n')readln();
			c = get_char();
		}
		if(c!=EOF){             //������'c'��ʼ����
			(*infile).putback(c);
			(*infile)>>course.course_no;
			(*infile)>>course.course_name;
			(*infile)>>course.period;
			(*infile)>>course.term;
			w=locate_vex(course.course_no);
			if(w==-1){
				cout<<"�Ƿ�����!"<<endl;
				exit(1);
			}
  
			c=get_char();
			while(c=='c'){
				(*infile).putback(c);
				(*infile)>>prio_course_no;
				v=locate_vex(prio_course_no);  //��ʾ��һ����<v,w>
				if(v==-1){
					cout<<"�Ƿ��Ⱦ�����!"<<endl;
					exit(1);
				}
				g.insert_arc(v,w);            //���뻡
				indegree[w]=indegree[w]+1;	  //����w��ȼ�1	
				c=get_char();
			}
		}
		if(c!='\n')readln();
		c=get_char();
	}

	if(course_total_num!=g.vex_num()){
		cout<<"��ѧ�������γ�������ʵ�ʿγ��������!"<<endl;
		exit(1);
	}

}


template<int terms_num,int max_size> 
void Range_Course<terms_num,max_size>::topological_order()
{	//����������ʽ�����ſ�
    int term,m=0;   
	CourseType course_v;
	int v;
	List<int> lq_arg,lt_arg;
	
	//�������Ϊ0����Ķ���q[0..terms_num]

	for(v=0;v<g.vex_num();v++){  
		//ȡ������v��������Ϣ
		if(g.get_vex(v,course_v)!=success){
			cout<<"�Ƿ�����!"<<endl;
			exit(1);
		}
		if(indegree[v]==0)  
			if(q[course_v.term].append(v)!=success){
				cout<<"�������!"<<endl;
				exit(1);
			}  //���Ϊ0�߽�����Ӧѧ�ڵĶ���
	}


	//�û��ݷ��ݹ鷽ʽ�ſ�,���ɿα�
	term=1;
	assign(lq_arg,q[0]);
	lt_arg.clear();

	if(recursive_topological_order(lq_arg,0,lt_arg,term,indegree)==false){
		cout<<"�ſ�ʱ���ֳ�ͻ!"<<endl;
		exit(1);
	}

}


template<int terms_num,int max_size> 
void Range_Course<terms_num,max_size>::write()
{	//����α���Ϣ
	int term,i,week_day;
	char s[20],s_tem[20];
	for(term=1;term<=terms_num;term++){
		
		//��ʾ��ͷ
		(*outfile)<<"��"<<decimal_to_chinese_char(term,s_tem)<<"ѧ�ڿα�"<<endl;
		(*outfile)<<endl;
		
		//��ʾ����
		write("�ڴ�",8);
		for(i=1;i<=5;i++)
			write(strcat(strcpy(s,"����"),decimal_to_chinese_char(i,s_tem)),18);
		(*outfile)<<endl<<endl;
		
		//��ʾ�α�����
		for(i=1;i<=10;i++){
			write(strcat(strcat(strcpy(s,"��"),decimal_to_chinese_char(i,s_tem)),"��"),8);
			for(week_day=1;week_day<=5;week_day++)
				write(course_table[term][i][week_day],18);
			(*outfile)<<endl;

			if(i==2||i==7)(*outfile)<<endl<<"�μ���Ϣ"<<endl<<endl;
			if(i==5)(*outfile)<<endl<<endl<<"�����Ϣ"<<endl<<endl<<endl;
			if(i==10)(*outfile)<<endl<<"����ϰ"<<endl<<endl;
		}

		//���α�֮���Խ϶�������
		(*outfile)<<endl<<endl<<endl<<endl<<endl<<endl;
	}
}


template<int terms_num,int max_size> 
int Range_Course<terms_num,max_size>::locate_vex(char course_no[])
{	//���ض�λ����
	int position=0;
	CourseType course;
	strcpy(course.course_no,course_no);
	return g.locate_vex(course);
}


template<int terms_num,int max_size> 
void Range_Course<terms_num,max_size>::readln()
{	//����   
	char c;
	while ((c = (*infile).peek()) != EOF && (c = (*infile).get()) != '\n');
}


template<int terms_num,int max_size> 
char Range_Course<terms_num,max_size>::get_char()
{	//��������������һ��������ַ�
	char c;
	while ((c = (*infile).peek()) != EOF && ((c = (*infile).get()) == ' '||c=='\t'));
	return c;
}


template<int terms_num,int max_size> 
bool Range_Course<terms_num,max_size>::range(int num,int term,char course_name[])
{	//�ſ�
	static int week_day=1; //Ҫ�ſε�����
	int i;
	if(num==1){     //��1�ڿν����ſ�
		if(range_one(week_day,term,course_name)==false)return false;
		else{
			week_day=(week_day<4)?(week_day+2):(week_day+2-5);
			return true;
		};
	}
	else if(num%3==0){
		for(i=0;i<num/3;i++)
			if(range_three(week_day,term,course_name)==false)return false;
			else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
		return true;
	}
	else if(num%3==1){
		for(i=0;i<num/3-1;i++)
			if(range_three(week_day,term,course_name)==false)return false;
			else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
		for(i=0;i<2;i++)
			if(range_two(week_day,term,course_name)==false)return false;
			else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
		return true;
	}
	else{
		for(i=0;i<num/3;i++)
			if(range_three(week_day,term,course_name)==false)return false;
			else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
		if(range_two(week_day,term,course_name)==false)	return false;
		else{
			week_day=(week_day<4)?(week_day+2):(week_day+2-5);
			return true;
		}
	}
}


template<int terms_num,int max_size> 
bool Range_Course<terms_num,max_size>::range_one(int &week_day,int term,char course_name[])
{	//��һ���1�ڿ�
	int i,k;
	for(k=0;k<5;k++){
		i=find_one(week_day,term);
		if(i==-1)
			week_day=(week_day<4)?(week_day+2):(week_day+2-5);
		else{
			strcpy(course_table[term][i][week_day],course_name);
			return true; 
		}
	}
	return false;
}


template<int terms_num,int max_size> 
bool Range_Course<terms_num,max_size>::range_two(int &week_day,int term,char course_name[])
{	//��һ���2�ڿ�
	int i,k,week_day_tem;
	for(k=0;k<5;k++){
		i=find_two(week_day,term);
		if(i==-1)
			week_day=(week_day<4)?(week_day+2):(week_day+2-5);
		else{
			strcpy(course_table[term][i][week_day],course_name);
			strcpy(course_table[term][i+1][week_day],course_name);
			return true; 
		}
	}
	//��2�������ϵĿηֽ�Ϊ���������ϵ�1�ڿ�
	week_day_tem=week_day;
	if(range_one(week_day_tem,term,"")==true&&range_one(week_day_tem,term,"")==true){
		range_one(week_day,term,course_name);
		range_one(week_day,term,course_name);
		return true;
	}
	return false;
}


template<int terms_num,int max_size> 
bool Range_Course<terms_num,max_size>::range_three(int &week_day,int term,char course_name[])
{	//��һ���3�ڿ�
	int i,k,week_day_tem;
	for(k=0;k<5;k++){
		i=find_three(week_day,term);
		if(i==-1)
			week_day=(week_day<4)?(week_day+2):(week_day+2-5);
		else{
			strcpy(course_table[term][i][week_day],course_name);
			strcpy(course_table[term][i+1][week_day],course_name);
			strcpy(course_table[term][i+2][week_day],course_name);
			return true; 
		}
	}
	//��3�������ϵĿηֽ�Ϊ2�������ϵĿ��뵥���ϵ�1�ڿ�
	week_day_tem=week_day;
	if(range_one(week_day_tem,term,"")==true&&range_two(week_day_tem,term,"")==true){
		range_one(week_day,term,course_name);
		range_two(week_day,term,course_name);
		return true;
	}
	return false;
}


template<int terms_num,int max_size> 
int Range_Course<terms_num,max_size>::find_one(int week_day,int term)
{	//�Ҳ�һ��1�ڿտ�,�����пյĽڴΣ�����-1��ʾ��������
	int i;
	for(i=1;i<=8;i++)   //����1��3����Ƿ��п�
		if((i<=2||(i>=6&&i<=7))&&strlen(course_table[term][i][week_day])==0)
			return i;
	for(i=1;i<=8;i++)   //����2��4����Ƿ��п�
		if((i>=3&&i<=5||(i>=8))&&strlen(course_table[term][i][week_day])==0)
			return i;
	return -1;
}


template<int terms_num,int max_size> 
int Range_Course<terms_num,max_size>::find_two(int week_day,int term)
{	//�Ҳ�һ������2�ڿտ�,��������2�ڿ��пյĵ�1�ڿεĽڴΣ�����-1��ʾ��������
	if(strlen(course_table[term][1][week_day])==0&&strlen(course_table[term][2][week_day])==0)
		return 1;   //����1����Ƿ��п�
	if(strlen(course_table[term][6][week_day])==0&&strlen(course_table[term][7][week_day])==0)
		return 6;   //����3����Ƿ��п�
	if(strlen(course_table[term][3][week_day])==0&&strlen(course_table[term][4][week_day])==0)
		return 3;   //����2����Ƿ��п�
	if(strlen(course_table[term][4][week_day])==0&&strlen(course_table[term][5][week_day])==0)
		return 4;   //����2����Ƿ��п�
	if(strlen(course_table[term][8][week_day])==0&&strlen(course_table[term][9][week_day])==0)
		return 8;   //����4����Ƿ��п�
	if(strlen(course_table[term][9][week_day])==0&&strlen(course_table[term][10][week_day])==0)
		return 9;   //����4����Ƿ��п�
	return -1;
}


template<int terms_num,int max_size> 
int Range_Course<terms_num,max_size>::find_three(int week_day,int term)
{	//�Ҳ�һ������3�ڿտ�,��������3�ڿ��пյĵ�1�ڿεĽڴΣ�����-1��ʾ��������
	if(strlen(course_table[term][3][week_day])==0&&strlen(course_table[term][4][week_day])==0&&strlen(course_table[term][5][week_day])==0)
		return 3;   //����2����Ƿ��п�
	if(strlen(course_table[term][8][week_day])==0&&strlen(course_table[term][9][week_day])==0&&strlen(course_table[term][10][week_day])==0)
		return 8;   //����3����Ƿ��п�
	return -1;
}


template<int terms_num,int max_size> 
void Range_Course<terms_num,max_size>::write(char s[],int l)
{	//���ָ�����ȵ��ַ��� 
	int i;
	(*outfile)<<s;
	for(i=0;i<l-strlen(s);i++)(*outfile)<<" ";
}


template<int terms_num,int max_size> 
char *Range_Course<terms_num,max_size>::decimal_to_chinese_char(int n,char *s)
{	//�����ı�ʾʮ������ 
	char digital[11][3]={"��","һ","��","��","��","��","��","��","��","��","ʮ"};
	s[0]='\0';

	if(n<0){
		strcat(s,"��");
		n=-n;
	}
	n=n%100;      //����n�����һ����λ��
	if(n/10>1){
		strcat(s,digital[n/10]);
		strcat(s,digital[10]);
		if(n%10>0)
			strcat(s,digital[n%10]);
	}
	else if(n/10==1){
		strcat(s,digital[10]);
		if(n%10>0)
			strcat(s,digital[n%10]);
	}
	else strcpy(s,digital[n%10]);
	return s;
}


template<int terms_num,int max_size> 
bool Range_Course<terms_num,max_size>::recursive_topological_order(const List<int> &lq,int start,const List<int> &lt,int term,int ind[])
{	/* �õݹ�ݹ���ٷ�ʽ�ſ�
	   lq:������q[0]������Ԫ����ɵ����Ա�
	   start:ȡ���¶�����ŵ���ʼλ��
	   lt:ȡ���Ķ����������ɵ����Ա�
	   term:��ǰ���ڴ����ѧ��
	   ind[]:��������
    */
	List<int> lq_arg,lt_arg;
	int i,j,position,end,x,size0,size_term,v,w;
	CourseType course_v,course_w;
	size0=q[0].size();
	size_term=q[term].size();

	if(lt.size()==0){
		if(size0+size_term<course_num[term]) return false;
		if(size_term>course_num[term]) return false;
	}

	//��lt.size()<course_num[term]-size_termʱ��ȡ���¶������
	if(lt.size()<course_num[term]-size_term){
		end=lq.size()-(course_num[term]-size_term-lt.size());
		for(position=start;position<=end;position++){
			lq_arg=lq;
			lt_arg=lt;
			if(lq_arg.remove(position,x)!=success){
				cout<<"�ſ�ʱ���ֳ�ͻ!"<<endl;
				exit(1);
			}
			lt_arg.insert(lt_arg.size(),x);
			if(recursive_topological_order(lq_arg,position,lt_arg,term,ind)==true)
				return true;
		}
		return false;
	}
	else{
		//��ʱlt.size()=course_num[term]-size_term
		//��q[term]�ж������ȡ������
		clear_course_table(term);//��յ�termѧ�ڵĿα�
		assign(q[0],lq);
		for(i=0;i<size_term;i++){
			q[term].retrieve(v);
			q[term].serve();

			//ȡ������v��������Ϣ
			if(g.get_vex(v,course_v)!=success){
				cout<<"�Ƿ�����!"<<endl;
				exit(1);
			}
			range(course_v.period,term,course_v.course_name);

			j=0;
			while(g.adj_vex(v,j++,w)==success){
				//ȡ������w��������Ϣ
				if(g.get_vex(w,course_w)!=success){
					cout<<"�Ƿ�����!"<<endl;
					exit(1);
				}
				ind[w]=ind[w]-1;  //����w��ȼ�1  
				if(ind[w]==0&&(course_w.term==0||course_w.term>term))
					q[course_w.term].append(w);
				else if(ind[w]==0&&(course_w.term>0||course_w.term<=term))
					return false;
			}
		}
		//��lt�ж������ȡ������
		for(i=0;i<course_num[term]-size_term;i++){
			lt.retrieve(i,v);

			//ȡ������v��������Ϣ
			if(g.get_vex(v,course_v)!=success){
				cout<<"�Ƿ�����!"<<endl;
				exit(1);
			}
			range(course_v.period,term,course_v.course_name);

			j=0;
			while(g.adj_vex(v,j++,w)==success){
				//ȡ������w��������Ϣ
				if(g.get_vex(w,course_w)!=success){
					cout<<"�Ƿ�����!"<<endl;
					exit(1);
				}
				ind[w]=ind[w]-1;  //����w��ȼ�1  
				if(ind[w]==0&&(course_w.term==0||course_w.term>term))
					q[course_w.term].append(w);
				else if(ind[w]==0&&(course_w.term>0||course_w.term<=term))
					return false;
			}
		}


		if(term==terms_num)return true; //��ʱ�ſν���

		//����һѧ�ڽ����ſ�
		term++;
		assign(lq_arg,q[0]);
		lt_arg.clear();
		return recursive_topological_order(lq_arg,0,lt_arg,term,ind);
	}
}


template<int terms_num,int max_size> 
void Range_Course<terms_num,max_size>::clear_course_table(int term)
{	//��յ�termѧ�ڵĿα�
	int i,j;
	for(i=1;i<=10;i++)							//�ڴ�
		for(j=1;j<=5;j++)						//����   
			strcpy(course_table[term][i][j],"");//��տα� 
}
