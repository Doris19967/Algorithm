#include"Utility.h"
#include"Sq_queue.h"
#include"Lk_list.h"
#include"Adjacency_lists.h"
#include"Range_course.h"

void main(int argc, char *argv[])
{
	char infile_name[256],outfile_name[256];
	
	if (argc > 1) strcpy(infile_name,argv[1]);
	else strcpy(infile_name,"course_inf.txt");
	ifstream file_in(infile_name);    //  �������������

    if (file_in == 0) {
		cout << "���ܴ򿪿γ���Ϣ�ļ� " << infile_name << endl;
        exit (1);
    }
   
	if (argc > 2) strcpy(outfile_name,argv[2]);
	else strcpy(outfile_name,"curriculum_scedule.txt");
	ofstream file_out(outfile_name);   //  ������������

    if (file_out == 0) {
		cout << "���ܴ򿪿γ����ļ� " << outfile_name << endl;
        exit (1);
    }

	cout<<"�γ���Ϣ�ļ�Ϊ: "<<infile_name<<endl<<endl;
	cout<<"�����ſΣ����Ժ�..."<<endl<<endl;

	//Range_Course<8,1000>��ʾ��8ѧ�ڿ�(�ӵ�1ѧ�ڵ���8ѧ��),����ܶ�1000�ſν����ſ�
	Range_Course<8,1000> rc(&file_in, &file_out);

	rc.read();               //����γ���Ϣ
	rc.topological_order();  //�������ŵ�˼�����ſ�
	rc.write();              //����α�

	cout<<"�ſν������α��ļ�Ϊ: "<<outfile_name<<endl;
	cout<<"�������˳�"<<endl;
	getch();

}
