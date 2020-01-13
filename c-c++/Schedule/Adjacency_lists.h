template<class VertexType>
struct VNode{
	VertexType data;             //��������Ԫ��ֵ֮
	List<int> arc_list;          //�ɻ���ɵ����Ա�
};


template<class VertexType,int max_size>
class Digraph {
public:
	//���캯��
	Digraph();
	//�����������ͷ��ڴ�ռ�
	~Digraph();
	//ȷ������Ԫ�ص�ֵΪitem�����
	int locate_vex(const VertexType &item);       
	//����ͼ�Ķ������
	int vex_num();       
	//�󶥵�v������Ԫ��ֵ
	Error_code get_vex(int v,VertexType &item);    
	//���ö���v������Ԫ��ֵ
	Error_code put_vex(int v,const VertexType &item);    
	//�󶥵�v�ĵ�i���ڽӵ� 
	Error_code adj_vex(int v,int i,int &adj_point);
	//��������Ԫ��ֵΪitem�Ķ���
	Error_code insert_vex(const VertexType &item);
	//���붥����ŷֱ�Ϊv,w�Ļ�
	Error_code insert_arc(int v,int w);
	//ɾ���������Ϊv�Ķ���
	Error_code remove_vex(int v);
	//ɾ��������ŷֱ�Ϊv,w�Ļ�
	Error_code remove_arc(int v,int w);
protected:
	int count;    //�����������Ϊmax_size
	VNode<VertexType> vertices[max_size]; 
	//��Ԫ��ֵΪentry�Ľ�����(-1��ʾʧ��)
	int index(const List<int> &l,int entry); 
};
   

template<class VertexType,int max_size>
Digraph<VertexType,max_size>::Digraph()
{   //���캯�� 
	count = 0;
}


template<class VertexType,int max_size>
Digraph<VertexType,max_size>::~Digraph()
{   //�����������ͷ��ڴ�ռ�
	int v;
	for(v=0;v<count;v++)
		vertices[v].arc_list.clear();
}


template<class VertexType,int max_size>
int Digraph<VertexType,max_size>::locate_vex(const VertexType &item)
{   //ȷ������Ԫ�ص�ֵΪitem�����
	int position=0;
	while(position<count && vertices[position].data!=item)position++;
	if(position<count)return position;   //��λ�ɹ�
	else return -1;                      //��λʧ��
}


template<class VertexType,int max_size>
int Digraph<VertexType,max_size>::vex_num()
{   //����ͼ�Ķ������
	return count;
}


template<class VertexType,int max_size>
Error_code Digraph<VertexType,max_size>::get_vex(int v,VertexType &item)
{   //�󶥵�v������Ԫ��ֵ
	if(v<count){
		item=vertices[v].data;
		return success;
	}
	else return range_error;
}


template<class VertexType,int max_size>
Error_code Digraph<VertexType,max_size>::put_vex(int v,const VertexType &item)
{   //���ö���v������Ԫ��ֵ
	if(v<count){
		vertices[v].data=item;
		return success;
	}
	else return range_error;
}


template<class VertexType,int max_size>
Error_code Digraph<VertexType,max_size>::adj_vex(int v,int i,int &adj_point)
{   //�󶥵�v�ĵ�i���ڽӵ� 
	if(v<count)return vertices[v].arc_list.retrieve(i,adj_point);
	else return range_error;
}


template<class VertexType,int max_size>
Error_code Digraph<VertexType,max_size>::insert_vex(const VertexType &item)
{   //��������Ԫ��ֵΪitem�Ķ���
	if(count<max_size){
		if(locate_vex(item)==-1){
			vertices[count++].data=item;
			return success;
		}
		else return duplicate;
	}
	else return overflow;
}


template<class VertexType,int max_size>
Error_code Digraph<VertexType,max_size>::insert_arc(int v,int w)
{   //���붥����ŷֱ�Ϊv,w�Ļ�
	if(v<count && w<count){
		if(index(vertices[v].arc_list,w)!=-1)
			return duplicate;
		else{
			vertices[v].arc_list.insert(vertices[v].arc_list.size(),w);
			return success;
		}
	}
	else return range_error;
}


template<class VertexType,int max_size>
Error_code Digraph<VertexType,max_size>::remove_vex(int v)
{   //ɾ���������Ϊv�Ķ���
	if(v<count){
		vertices[v].arc_list.clear();
		for(int i=v+1;i<count;i++)
			vertices[i-1]=vertices[i];
		count--;
		return success;
	}
	else return range_error;
}


template<class VertexType,int max_size>
Error_code Digraph<VertexType,max_size>::remove_arc(int v,int w)
{   //ɾ��������ŷֱ�Ϊv,w�Ļ�
	int position,x;
	if(v<count && w<count){
		if((position=index(vertices[v].arc_list,w))!=-1){
			vertices[v].arc_list.remove(position,x);
			return success;
		}
		else return fail;
	}
	else return range_error;
}


template<class VertexType,int max_size>
int Digraph<VertexType,max_size>::index(const List<int> &l,int entry)
{   //��Ԫ��ֵΪentry�Ľ�����
	int x, position=0;
	while(l.retrieve(position,x)==success && (x!=entry))position++;
	if(position<l.size())return position;
	else return -1;
}

