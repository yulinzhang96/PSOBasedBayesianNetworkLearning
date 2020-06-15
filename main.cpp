#include <iostream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
//#include <windows.h>
#include <set>

using namespace std;

#define N 99//����������ΪС��������2λ

//�洢����
vector<vector<float> > data;
//�洢��������������
vector<vector<float> > arranged_data;
//����������־
ofstream datalog;
//����������־
ofstream netlog;

class Particles
{
private:
    //��Ҷ˹���������ĵ�һ����
    vector<int> net_part_1;
    //��Ҷ˹���������ĵڶ����֣����ڵ㼯�ϣ�
    vector<vector<int> > net_part_2;
    //�ڵ�����
    int num_of_nodes;
    //ÿ���ڵ������󸸽ڵ���
    int max_parents;
public:
    //Ĭ�Ϲ��캯��
    Particles();
    //���캯��
    Particles(int num_of_nodes, int max_parents);
    //��ȡ���������������ĵ�һ����
    vector<int> GetPart1();
    //��ȡ���������������ĵڶ�����
    vector<vector<int> > GetPart2();
    //�������������������ĵ�һ����
    void SetPart1(const vector<int>& part1);
    //�������������������ĵڶ�����
    void SetPart2(const vector<vector<int> >& part2);
    //������ʼ��
    void Initialize();
    //����BIC���ֺ���ֵ
    vector<float> ComputeBIC();
    //��ӡ����
    void PrintParticle();
    //����������־
    void PrintLogToFile();
    //���������ṹ
    void PrintNetworkToFile();
};

Particles::Particles()
{
    num_of_nodes = 0;
    max_parents = 0;
    net_part_1.clear();
    net_part_2.clear();
}

Particles::Particles(int non, int mp)
{
    num_of_nodes = non;
    max_parents = mp;
    Initialize();
}

vector<int> Particles::GetPart1()
{
    return net_part_1;
}

vector<vector<int> > Particles::GetPart2()
{
    return net_part_2;
}

void Particles::SetPart1(const vector<int>& part1)
{
    net_part_1 = part1;
}

void Particles::SetPart2(const vector<vector<int> >& part2)
{
    net_part_2 = part2;
}

void Particles::Initialize()
{
    for(int i=1; i<=num_of_nodes; i++)
    {
        net_part_1.push_back(i);
    }
    srand((unsigned)time(NULL));
    for(int i=0; i<num_of_nodes; i++)
    {
        int number = net_part_1[i];
        bool* flag = new bool[number];
        for(int j=0; j<number; j++)
        {
            flag[j] = false;
        }
        vector<int> temp_part2;
        for(int k=0; k<max_parents; k++)
        {
            int temp = rand() % number;
            if(flag[temp]==false)
            {
                temp_part2.push_back(temp);
                flag[temp] = true;
            }
            else
            {
                temp_part2.push_back(0);
            }
            flag[0] = false;
        }
        net_part_2.push_back(temp_part2);
        temp_part2.clear();
        delete[] flag;
    }
}

void Particles::PrintParticle()
{
    cout<<"<";
    for(int i=0; i<num_of_nodes; i++)
    {
        cout<<net_part_1[i];
        if(i!= (num_of_nodes-1))
        {
            cout<<",";
        }
    }
    cout<<"|";
    for(int i=0; i<num_of_nodes; i++)
    {
        for(int j=0; j<max_parents; j++)
        {
            cout<<net_part_2[i][j];
            if(j!=(max_parents-1))
            {
                cout<<" ";
            }
        }
        if(i!= (num_of_nodes-1))
        {
            cout<<",";
        }
    }
    cout<<">"<<endl;
}

void Particles::PrintNetworkToFile()
{
    netlog<<"<";
    for(int i=0; i<num_of_nodes; i++)
    {
        netlog<<net_part_1[i];
        if(i!= (num_of_nodes-1))
        {
            netlog<<",";
        }
    }
    netlog<<"|";
    for(int i=0; i<num_of_nodes; i++)
    {
        for(int j=0; j<max_parents; j++)
        {
            netlog<<net_part_2[i][j];
            if(j!=(max_parents-1))
            {
                netlog<<" ";
            }
        }
        if(i!= (num_of_nodes-1))
        {
            netlog<<",";
        }
    }
    netlog<<">"<<endl;
}

void Particles::PrintLogToFile()
{
    datalog<<"<";
    for(int i=0; i<num_of_nodes; i++)
    {
        datalog<<net_part_1[i];
        if(i!= (num_of_nodes-1))
        {
            datalog<<",";
        }
    }
    datalog<<"|";
    for(int i=0; i<num_of_nodes; i++)
    {
        for(int j=0; j<max_parents; j++)
        {
            datalog<<net_part_2[i][j];
            if(j!=(max_parents-1))
            {
                datalog<<" ";
            }
        }
        if(i!= (num_of_nodes-1))
        {
            datalog<<",";
        }
    }
    datalog<<">"<<endl;
}

vector<float> Particles::ComputeBIC()
{
    vector<float> temp_BIC;
    for(int i=0; i<num_of_nodes; i++)
    {
        int qi =0, ri = 0;//qi���ڵ���ȡֵ״̬����ri�ӽڵ���ȡֵ״̬��
        bool flag = false;//�ڵ�i�Ƿ��и��ڵ�
        float first_part=0, second_part=0;
        vector<vector<int> > data_number;//�ӽڵ�i��ri��ȡֵ״̬�����ڵ㼯��qi��ȡֵ״̬����������
        ri = arranged_data[i].size();
        for(int j=0; j<net_part_2[i].size(); j++)
        {
            if(net_part_2[i][j]!=0)
            {
                qi = 1;//�����ڵ�i���ڵ���ȡֵ����
                flag = true;
                break;
            }
        }
        if(flag)//�ڵ�i�и��ڵ�
        {
            vector<int> parent_node_number;//��¼���ڵ㼯����
            vector<int> parent_node_value_range;//��¼���ڵ㼯ȡֵ����
            for(int j=0; j<net_part_2[i].size(); j++)//��ȷ
            {
                int temp_parent = net_part_2[i][j];
                if(temp_parent!=0)//������Ϊ0��Ϊ���ڵ�����
                {
                    int temp_size = arranged_data[temp_parent-1].size();//��ȡĳ�����ڵ���ȡֵ��
                    parent_node_number.push_back(temp_parent);//˳�����븸�ڵ�����
                    parent_node_value_range.push_back(temp_size);
                    qi *= temp_size;
                }
            }
            for(int m=0; m<ri; m++)//����ѭ��Ϊ�ӽڵ�ȡֵ��
            {
                vector<int> number;//�ڵ�iȡ��k��ֵʱ�����ڵ㼯ȡ��j������ֵ����������
                for(int n=0; n<qi; n++)//�ڲ�ѭ��Ϊ���ڵ㼯ȡֵ��
                {
                    int Nijk = 0;
                    float son_node_value = arranged_data[i][m];//��ȡ�ӽڵ���m��ֵ
                    vector<float> parent_node_values;//��¼���ڵ㼯��ȡֵ����
                    int shang = n, yushu = 0, temp_size = 0;
                    for(int j=0; j<parent_node_number.size(); j++)
                    {
                        temp_size = parent_node_value_range[j];//��ȡĳ�����ڵ���ȡֵ��
                        int temp_parent = parent_node_number[j];
                        yushu = shang%temp_size;//��ǰ���ڵ�ȡ��yushu��ֵ
                        parent_node_values.push_back(arranged_data[temp_parent-1][yushu]);
                        shang = shang/temp_size;
                    }
                    for(int p=0; p<data.size(); p++)
                    {
                        if(son_node_value == data[p][i])
                        {
                            int j;
                            for(j=0; j<parent_node_number.size(); j++)
                            {
                                int temp_parent = parent_node_number[j];//��ǰ���ڵ�����
                                if(parent_node_values[j] != data[p][temp_parent-1])
                                {
                                    break;
                                }
                            }
                            if(j == parent_node_number.size())
                            {
                                Nijk++;
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                    number.push_back(Nijk);
                }
                data_number.push_back(number);
                number.clear();
            }
            int Nij = 0;//���ݼ��о��е�i�������ĸ��ڵ��ĵ�j��ֵ���ϵ�������
            vector<int> Nij_vector;
            int hengxiang = data_number[0].size();
            for(int z=0; z<hengxiang; z++)
            {
                for(int y=0; y<data_number.size(); y++)
                {
                    Nij += data_number[y][z];
                }
                Nij_vector.push_back(Nij);
                Nij = 0;
            }
            for(int k=0; k<qi; k++)
            {
                for(int c=0; c<ri; c++)
                {
                    if(data_number[c][k]!=0)
                    {
                        first_part += data_number[c][k]*log(float(data_number[c][k])/Nij_vector[k]);
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            Nij_vector.clear();
            parent_node_number.clear();
        }
        else//�ڵ�i�޸��ڵ�
        {
            first_part = 0;
        }
        second_part = log(data.size())*(qi*(ri-1))/2;
        temp_BIC.push_back(first_part - second_part);
        data_number.clear();
    }
    return temp_BIC;
}

class Network
{
private:
    //ѡ�������ݼ�
    int dataset;
    //���Ӹ���
    int num_of_particles;
    //�ڵ���
    int num_of_nodes;
    //���󸸽ڵ�����
    int max_parents;
    //��������
    int iterations;
    //��¼���������ѵı�Ҷ˹��������
    vector<Particles> best_particles_network;
    //��¼�����ӵ�ǰ�ı�Ҷ˹��������
    vector<Particles> current_particles_network;
    //��¼���������е�ǰ��ȫ�����ѱ�Ҷ˹��������
    Particles best_particle;
    //��¼�����������ϴ�����ѧϰ��ȫ�����ѱ�Ҷ˹��������
    Particles old_best_particle;
    //��¼��ʱ��ȫ�����ѱ�Ҷ˹��������
    Particles temp_best_particle;
    //��¼���������ѵı�Ҷ˹������BICֵ
    vector<vector<float> > best_BIC;
    //��¼�����ӵ�ǰ��Ҷ˹������BICֵ
    vector<vector<float> > current_BIC;
    //��¼ȫ���������ӵ�BICֵ
    vector<float> global_best_BIC;
    //��¼�ϴ�����ѧϰȫ���������ӵ�BICֵ
    vector<float> old_global_best_BIC;
    //��ʱ��ȫ���������ӵ�BICֵ
    vector<float> temp_global_best_BIC;
    //�����ӵ��ٶ�
    vector<vector<int> > *velocity;
    //����ѧϰʱ�޸��ڵ����ӽڵ��ĸ��ʷֲ�
    vector<vector<float> > probability_a;
    //����ѧϰʱ�и��ڵ����ӽڵ��ĸ��ʷֲ�
    vector<vector<vector<float> > > probability_b;
    //�ո��ڵ㼯
    vector<int> zero;
    //�����ٶȸ��µĲ�������Щ����Ӱ��ѧϰЧ����������Ҫ�޸ģ�r1,r2Ϊ[0,1)֮����������
    float q, w, c1, r1, c2, r2;

public:
    //���캯���������ڵ���num_of_nodes��ÿ���ڵ������󸸽ڵ���max_parents
    Network(int ds, int nop, int mp, int its);
    //��������
    ~Network();
    //����Ⱥ��ʼ��
    void Initialize();
    //����Ԥ����
    void DataPreprocess();
    //��ȡ����
    void ReadData(const int& t);
    //��������
    void ArrangeData();
    //������ǰ�����ļ�������
    void ShowData();
    //������������������
    void ShowArrangedData();
    //��Ҷ˹����ѧϰ
    void NetworkLearning();
    //��Ҷ˹�����ṹѧϰ
    void StructureLearning(const int& times);
    //��Ҷ˹��������ѧϰ
    void ParameterLearning();
    //��ӡĳ�����ӵĵ�ǰ�ٶ�
    void PrintVelocity(const int& particle_seq);
    //����ĳ�����ӵĵ�ǰ�ٶȵ���־�ļ���
    void PrintVelocityToFile(const int& particle_seq);
    //����λ������һ������λ��������
    vector<vector<int> > PositionMinusPosition(const vector<vector<int> >& position_a, const vector<float>& BIC_a, const vector<float>& BIC_b);
    //�������ٶ�����
    vector<vector<int> > ConstMultiplyVelocity(const float& number_a, const vector<vector<int> >& velocity_b);
    //�ٶ����ٶ�����
    vector<vector<int> > VelocityPlusVelocity(const vector<vector<int> >& velocity_a, const vector<vector<int> >& velocity_b);
    //λ�����ٶ�����
    vector<vector<int> > PositionPlusVelocity(const vector<vector<int> >& position_a, const vector<vector<int> >& velocity_b);
    //����һ����Ҷ˹�������ڵ���BICֵ�ܺ�
    float ComputeBIC(const vector<float>& temp_BIC);
    //����Ȩ�����Ӻ���
    float WFunction(const int& times2);
    //�������Ӻ���
    float QFunction(const int& q1, const int& q2);
    //�¾������Ա�
    bool CompareNetwork(Particles p1, Particles p2);
    //��ʼ��������
    void InitializeNew();
    //�µĽṹѧϰ
    void NewStructureLearning(const int& times);
};

//���캯���������ڵ���num_of_nodes��ÿ���ڵ������󸸽ڵ���max_parents����������iterations
Network::Network(int ds, int nop, int mp, int its)
{
    dataset = ds;
    num_of_particles = nop;
    max_parents = mp;
    iterations = its;
    data.clear();
    arranged_data.clear();
    datalog.open("./Asia/result/log.txt",ofstream::out);
    if(!datalog)
    {
        cerr<<"datalog.txt open error!"<<endl;
        exit(1);
    }
    netlog.open("./Asia/result/network.txt",ofstream::out);
    if(!netlog)
    {
        cerr<<"netlog.txt open error!"<<endl;
        exit(1);
    }
    //DataPreprocess();
    ReadData(0);
    ArrangeData();//��������
    //ShowData();
    //ShowArrangedData();
    best_particle = Particles(num_of_nodes, max_parents);
    old_best_particle = Particles(num_of_nodes, max_parents);
    old_best_particle.SetPart2(best_particle.GetPart2());
    for(int i=0; i<num_of_nodes; i++)
    {
        global_best_BIC.push_back(-100000);
    }
    float total_global_best_BIC = ComputeBIC(global_best_BIC);
    Initialize();
    for(int i=0; i<num_of_particles; i++)
    {
        float temp_BIC_total = ComputeBIC(best_BIC[i]);
        vector<vector<int> > temp_part2 = best_particles_network[i].GetPart2();
        if(temp_BIC_total > total_global_best_BIC)
        {
            global_best_BIC = best_BIC[i];
            total_global_best_BIC = temp_BIC_total;
            best_particle.SetPart2(temp_part2);
        }
        temp_part2.clear();
    }
    //cout<<"Current global best particle:";
    datalog<<"Initial Information:"<<endl<<"Global best network:";
    netlog<<"Initial Information:"<<endl<<"Global best network:";
    //best_particle.PrintParticle();
    best_particle.PrintLogToFile();
    best_particle.PrintNetworkToFile();
    netlog<<"Global best BIC value = "<<total_global_best_BIC<<endl<<endl;
    //cout<<"Global best BIC value="<<total_global_best_BIC<<endl<<endl;
    datalog<<"Global best BIC value = "<<total_global_best_BIC<<endl<<endl;
    cout<<"******************************Batch[0]******************************"<<endl<<endl;
    datalog<<"******************************Batch[0]******************************"<<endl<<endl;
    netlog<<"******************************Batch[0]******************************"<<endl;
}

//��������
Network::~Network()
{
    best_particles_network.clear();
    current_particles_network.clear();
    best_BIC.clear();
    current_BIC.clear();
    global_best_BIC.clear();
    old_global_best_BIC.clear();
    probability_a.clear();
    probability_b.clear();
    zero.clear();
    delete[] velocity;
}

//������ʼ��
void Network::Initialize()
{
    srand((unsigned)time(NULL));
    cout<<endl<<"Initializing";
    datalog<<endl<<"Initializing";
    zero.clear();
    for(int i=0; i<max_parents; i++)
    {
        zero.push_back(0);
    }
    best_particles_network.clear();
    current_particles_network.clear();
    velocity = new vector<vector<int> >[num_of_particles];
    for(int i=0; i<num_of_particles; i++)
    {
        Particles particle = Particles(num_of_nodes, max_parents);
        //cout<<"Particle["<<i+1<<"]:";
        datalog<<"Particle["<<i+1<<"]:";
        current_particles_network.push_back(particle);
        best_particles_network.push_back(particle);//��ʼ��ʱÿ�����ӵ�����λ�þ��ǵ�ǰ������λ��
        //current_particles_network[i].PrintParticle();
        current_particles_network[i].PrintLogToFile();
        vector<float> temp_BIC = particle.ComputeBIC();//����BICֵ
        best_BIC.push_back(temp_BIC);
        current_BIC.push_back(temp_BIC);
        vector<vector<int> > temp_temp_velocity;//��ʼ�������ٶ�
        vector<int> temp_velocity;
        for(int i=0; i<num_of_nodes; i++)
        {
            int number = i+1;
            bool* flag = new bool[number];
            for(int j=0; j<number; j++)
            {
                flag[j] = false;
            }
            for(int k=0; k<max_parents; k++)
            {
                int temp = rand() % number;
                if(flag[temp]==false)
                {
                    temp_velocity.push_back(temp);
                    flag[temp] = true;
                }
                else
                {
                    temp_velocity.push_back(0);
                }
                flag[0] = false;
            }
            temp_temp_velocity.push_back(temp_velocity);
            temp_velocity.clear();
            delete[] flag;
        }
        temp_BIC.clear();
        velocity[i] = temp_temp_velocity;
        temp_temp_velocity.clear();
        //PrintVelocity(i);//�������ӵ��ٶ�
        PrintVelocityToFile(i);
        float temp_BIC_total = ComputeBIC(best_BIC[i]);
        //cout<<"Best BIC value=BIC value="<<temp_BIC_total<<endl<<endl;
        datalog<<"Best BIC value=BIC value="<<temp_BIC_total<<endl<<endl;
    }
}

//����Ԥ����
void Network::DataPreprocess()
{
    if(dataset==1)
    {
        ifstream inFile("./Asia/train_data.csv", ios::in);
        string lineStr;
        int pre_data[3000][8];
        string tag[8]={"VisitAsia","Smoking","Tuberculosis","Cancer","Bronchitis","TbOrCa","Dyspnea","XRay"};
        int column = 0;
        if(!inFile)
        {
            cout<<"Failed to read file!"<<endl;
        }
        getline(inFile, lineStr);
        int ct = 0;
        while(getline(inFile, lineStr)) // �� inFile �ж�ȡһ�У� �ŵ� lineStr ��
        {
            stringstream ss(lineStr); //��ȡ���ݷ����� ss�� ��, �����൱�ڳ�ʼ��
            string str;
            vector<string> line;
            // ���ն��ŷָ�
            while(getline(ss, str, ',')) // ss �У� ���� ������ ���ŷָss �ָ���һ����str
            {
                line.push_back(str);// ���ַ������õ� line Array
            }
            for(int i=0; i<line.size(); i++)
            {
                if(i==1)
                {
                    if(line[i]=="absent")
                        pre_data[ct][2]=0;
                    else if(line[i]=="present")
                        pre_data[ct][2]=1;
                }
                if(i==2)
                {
                    if(line[i]=="normal")
                        pre_data[ct][7]=1;
                    else if(line[i]=="abnormal")
                        pre_data[ct][7]=0;
                }
                if(i==3)
                {
                    if(line[i]=="FALSE")
                        pre_data[ct][5]=0;
                    else if(line[i]=="TRUE")
                        pre_data[ct][5]=1;
                }
                if(i==4)
                {
                    if(line[i]=="absent")
                        pre_data[ct][3]=0;
                    else if(line[i]=="present")
                        pre_data[ct][3]=1;
                }
                if(i==5)
                {
                    if(line[i]=="absent")
                        pre_data[ct][6]=0;
                    else if(line[i]=="present")
                        pre_data[ct][6]=1;
                }
                if(i==6)
                {
                    if(line[i]=="absent")
                        pre_data[ct][4]=0;
                    else if(line[i]=="present")
                        pre_data[ct][4]=1;
                }
                if(i==7)
                {
                    if(line[i]=="no_visit")
                        pre_data[ct][0]=0;
                    else if(line[i]=="visit")
                        pre_data[ct][0]=1;
                }
                if(i==8)
                {
                    if(line[i]=="smoker")
                        pre_data[ct][1]=1;
                    else if(line[i]=="non_smoker")
                        pre_data[ct][1]=0;
                }
            }
            ct++;
            line.clear();
        }
        inFile.close();
        ofstream outFile("./Asia/new_data.csv", ios::out);
        if(!outFile)
        {
            cerr<<"open error!"<<endl;
            exit(1);
        }
        for(int i=0; i<3000; i++)
        {
            for(int j=0; j<8; j++)
            {
                outFile<<pre_data[i][j];
                if(j!=7)
                    outFile<<",";
            }
            outFile<<endl;
        }
        outFile.close();
    }
}

//��ȡ����
void Network::ReadData(const int& t)
{
    if(dataset==1)
    {
        ifstream inFile("./Asia/new_data.csv", ios::in);
        string lineStr;
        int num = 500;//��������������
        int column = 0;
        if(!inFile)
        {
            cout<<"Failed to read file!"<<endl;
        }
        for(int i=0; i<t*num; i++)//�Թ�ǰ��(t-1)*3000/times������
        {
            getline(inFile, lineStr);
        }
        while(getline(inFile, lineStr)) // �� inFile �ж�ȡһ�У� �ŵ� lineStr ��
        {
            if(num==0)
            {
                break;
            }
            stringstream ss(lineStr); //��ȡ���ݷ����� ss�� ��, �����൱�ڳ�ʼ��
            string str;
            vector<float> line;
            // ���ն��ŷָ�
            while(getline(ss, str, ',')) // ss �У� ���� ������ ���ŷָss �ָ���һ����str
            {
                float temp = stof(str);
                line.push_back(temp);// ���ַ������õ� line Array
                column++;
            }
            data.push_back(line);
            line.clear();
            num--;
        }
        if(t==0)
            num_of_nodes = column/data.size();//���ýڵ������������ݵ�������ÿһ�д���һ���ڵ�
        while(num_of_nodes <= max_parents)
        {
            cout<<"The maximum number of parents of each node must be less than the number of nodes!"<<endl<<"Please input again!"<<endl;
            cout<<"max_parents=";
            cin>>max_parents;
            cout<<endl;
        }
        inFile.close();
    }
}

//��������
void Network::ArrangeData()
{
    arranged_data.clear();
    for(int i=0; i<num_of_nodes; i++)
    {
        vector<float> temp_data;
        for(int j=0; j<data.size(); j++)
        {
            if(temp_data.empty())
            {
                temp_data.push_back(data[j][i]);
            }
            else
            {
                int k;
                for(k=0; k<temp_data.size(); k++)
                {
                    if(data[j][i]==temp_data[k])
                    {
                        break;
                    }
                }
                if(k==temp_data.size())
                {
                    temp_data.push_back(data[j][i]);
                }
            }
        }
        arranged_data.push_back(temp_data);
        temp_data.clear();
    }
}

//������ǰ�����ļ�������
void Network::ShowData()
{
    cout<<"Raw Data:"<<endl;
    for(int i=0; i<data.size(); i++)
    {
        for(int j=0; j<data[i].size(); j++)
        {
            cout<<data[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

//������������������
void Network::ShowArrangedData()
{
    cout<<"Arranged Data:"<<endl;
    for(int i=0; i<num_of_nodes; i++)
    {
        cout<<"Node["<<i+1<<"]:";
        for(int j=0; j<arranged_data[i].size(); j++)
        {
            cout<<arranged_data[i][j]<<" ";
        }
        cout<<endl;
    }
}

//��Ҷ˹����ѧϰ
void Network::NetworkLearning()
{
    int i = 1;
    do
    {
        cout<<"Loop["<<i<<"]"<<endl<<endl;
        datalog<<"Loop["<<i<<"]"<<endl<<endl;
        StructureLearning(i);
        i++;
    }while(i<=iterations);
    old_best_particle.SetPart2(best_particle.GetPart2());
    old_global_best_BIC = global_best_BIC;
    ParameterLearning();
    temp_best_particle = Particles(num_of_nodes, max_parents);
    for(int j=1; j<=5; j++)
    {
        cout<<"******************************Batch["<<j<<"]******************************"<<endl<<endl;
        datalog<<"******************************Batch["<<j<<"]******************************"<<endl<<endl;
        netlog<<"******************************Batch["<<j<<"]******************************"<<endl;
        old_best_particle.SetPart2(best_particle.GetPart2());
        old_global_best_BIC = global_best_BIC;
        arranged_data.clear();
        data.clear();
        ReadData(j);
        ArrangeData();
        InitializeNew();
        for(int k=1; k<=iterations; k++)
        {
            cout<<"Loop["<<k<<"]"<<endl<<endl;
            datalog<<"Loop["<<k<<"]"<<endl<<endl;
            StructureLearning(k);
        }
        temp_best_particle.SetPart2(best_particle.GetPart2());
        temp_global_best_BIC.clear();
        temp_global_best_BIC = global_best_BIC;
        InitializeNew();
        for(int k=1; k<=iterations; k++)
        {
            cout<<"Loop["<<k<<"]"<<endl<<endl;
            datalog<<"Loop["<<k<<"]"<<endl<<endl;
            NewStructureLearning(k);
        }
        ParameterLearning();
    }
    datalog.close();
    netlog.close();
}

void Network::InitializeNew()
{
    best_particles_network.clear();
    current_particles_network.clear();
    best_BIC.clear();
    current_BIC.clear();
    global_best_BIC.clear();
    delete[] velocity;
    best_particle = Particles(num_of_nodes, max_parents);
    for(int i=0; i<num_of_nodes; i++)
    {
        global_best_BIC.push_back(-100000);
    }
    float total_global_best_BIC = ComputeBIC(global_best_BIC);
    Initialize();
    for(int i=0; i<num_of_particles; i++)
    {
        float temp_BIC_total = ComputeBIC(best_BIC[i]);
        vector<vector<int> > temp_part2 = best_particles_network[i].GetPart2();
        if(temp_BIC_total > total_global_best_BIC)
        {
            global_best_BIC = best_BIC[i];
            total_global_best_BIC = temp_BIC_total;
            best_particle.SetPart2(temp_part2);
        }
        temp_part2.clear();
    }
    //cout<<"Current global best particle:";
    datalog<<"Initial Information:"<<endl<<"Global best network:";
    netlog<<"Initial Information:"<<endl<<"Global best network:";
    //best_particle.PrintParticle();
    best_particle.PrintLogToFile();
    best_particle.PrintNetworkToFile();
    netlog<<"Global best BIC value = "<<total_global_best_BIC<<endl<<endl;
    //cout<<"Global best BIC value="<<total_global_best_BIC<<endl<<endl;
    datalog<<"Global best BIC value = "<<total_global_best_BIC<<endl<<endl;
}

//��Ҷ˹�����ṹѧϰ
void Network::StructureLearning(const int& times)
{
    srand((unsigned)time(NULL));
    w = WFunction(times);
    c1 = c2 = 2.2;
    q = QFunction(c1, c2);
    for(int i=0; i<num_of_particles; i++)
    {
        vector<vector<int> > new_velocity;//��һ�׶εı������ٶ�
        vector<vector<int> > old_velocity = velocity[i];//���׶εı������ٶ�
        vector<vector<int> > p_current = current_particles_network[i].GetPart2();//�����ӱ��׶ε�λ��
        vector<vector<int> > p_best = best_particles_network[i].GetPart2();//�����ӱ��׶����ѵ�λ��
        vector<vector<int> > g_best = best_particle.GetPart2();//���׶�ȫ���������ӵ�λ��

        r1 = rand() % (N + 1) / (float)(N + 1);
        r2 = rand() % (N + 1) / (float)(N + 1);
        vector<vector<int> > part_1 = ConstMultiplyVelocity(w, old_velocity);
        vector<vector<int> > temp_part_2 = PositionMinusPosition(p_best, best_BIC[i], current_BIC[i]);
        vector<vector<int> > part_2 = ConstMultiplyVelocity(c1*r1, temp_part_2);
        vector<vector<int> > temp_part_3 = PositionMinusPosition(g_best, global_best_BIC, current_BIC[i]);
        vector<vector<int> > part_3 = ConstMultiplyVelocity(c2*r2, temp_part_3);

        new_velocity = VelocityPlusVelocity(part_1, part_2);
        new_velocity = VelocityPlusVelocity(new_velocity, part_3);
        new_velocity = ConstMultiplyVelocity(q, new_velocity);
        velocity[i] = new_velocity;

        p_current = PositionPlusVelocity(p_current, new_velocity);
        current_particles_network[i].SetPart2(p_current);

        new_velocity.clear();
        old_velocity.clear();
        p_current.clear();
        p_best.clear();
        g_best.clear();

        part_1.clear();
        temp_part_2.clear();
        part_2.clear();
        temp_part_3.clear();
        part_3.clear();
    }
    //���¼��������ӵ�BICֵ��ͬʱ���¸����ӵ�����BICֵ��λ��
    for(int i=0; i<num_of_particles; i++)
    {
        current_BIC[i] = current_particles_network[i].ComputeBIC();//�˴���ComputeBIC����Ϊ��Particle�ĺ���
        if(ComputeBIC(current_BIC[i])>ComputeBIC(best_BIC[i]))
        {
            vector<vector<int> > temp_part2 = current_particles_network[i].GetPart2();
            best_particles_network[i].SetPart2(temp_part2);
            best_BIC[i] = current_BIC[i];
            temp_part2.clear();
        }
    }
    //�Ƚ����������е�BICֵ������ȫ���������ӵ�BICֵ��λ��
    for(int i=0; i<num_of_particles; i++)
    {
        if(ComputeBIC(best_BIC[i])>ComputeBIC(global_best_BIC))
        {
            vector<vector<int> > temp_part2 = best_particles_network[i].GetPart2();
            best_particle.SetPart2(temp_part2);
            global_best_BIC = best_BIC[i];
            temp_part2.clear();
        }
    }
    //�������º�����Ϣ
    for(int i=0; i<num_of_particles; i++)
    {
        //cout<<"Particle["<<i+1<<"]:"<<endl;
        //cout<<"Current network:";
        //current_particles_network[i].PrintParticle();
        //cout<<"Current BIC value = "<<ComputeBIC(current_BIC[i])<<endl;
        //PrintVelocity(i);
        //cout<<"Best network:";
        //best_particles_network[i].PrintParticle();
        //cout<<"Best BIC value = "<<ComputeBIC(best_BIC[i])<<endl<<endl;

        datalog<<"Particle["<<i+1<<"]:"<<endl;
        datalog<<"Current network:";
        current_particles_network[i].PrintLogToFile();
        datalog<<"Current BIC value = "<<ComputeBIC(current_BIC[i])<<endl;
        PrintVelocityToFile(i);
        datalog<<"Best network:";
        best_particles_network[i].PrintLogToFile();
        datalog<<"Best BIC value = "<<ComputeBIC(best_BIC[i])<<endl<<endl;
    }
    cout<<endl<<"Global best network:";
    best_particle.PrintParticle();
    cout<<"Global best BIC value = "<<ComputeBIC(global_best_BIC)<<endl<<endl<<endl;

    datalog<<"Global best network:";
    best_particle.PrintLogToFile();
    datalog<<"Global best BIC value = "<<ComputeBIC(global_best_BIC)<<endl<<endl<<endl;

    if(times==iterations)
    {
        netlog<<"Global best network:";
        best_particle.PrintNetworkToFile();
        netlog<<"Global best BIC value = "<<ComputeBIC(global_best_BIC)<<endl;
    }
}

void Network::NewStructureLearning(const int& times)
{
    srand((unsigned)time(NULL));
    w = WFunction(times);
    c1 = c2 = 2.2;
    q = QFunction(c1, c2);
    for(int i=0; i<num_of_particles; i++)
    {
        vector<vector<int> > new_velocity;//��һ�׶εı������ٶ�
        vector<vector<int> > old_velocity = velocity[i];//���׶εı������ٶ�
        vector<vector<int> > p_current = current_particles_network[i].GetPart2();//�����ӱ��׶ε�λ��
        vector<vector<int> > BN_old_best = old_best_particle.GetPart2();//�������ṹ���ѵ�λ��
        vector<vector<int> > BN_new_best = temp_best_particle.GetPart2();//���׶�ȫ���������ӵ�λ��

        r1 = rand() % (N + 1) / (float)(N + 1);
        r2 = rand() % (N + 1) / (float)(N + 1);
        vector<vector<int> > part_1 = ConstMultiplyVelocity(w, old_velocity);
        vector<vector<int> > temp_part_2 = PositionMinusPosition(BN_old_best, old_global_best_BIC, current_BIC[i]);
        vector<vector<int> > part_2 = ConstMultiplyVelocity(c1*r1, temp_part_2);
        vector<vector<int> > temp_part_3 = PositionMinusPosition(BN_new_best, temp_global_best_BIC, current_BIC[i]);
        vector<vector<int> > part_3 = ConstMultiplyVelocity(c2*r2, temp_part_3);

        new_velocity = VelocityPlusVelocity(part_1, part_2);
        new_velocity = VelocityPlusVelocity(new_velocity, part_3);
        new_velocity = ConstMultiplyVelocity(q, new_velocity);
        velocity[i] = new_velocity;

        p_current = PositionPlusVelocity(p_current, new_velocity);
        current_particles_network[i].SetPart2(p_current);

        new_velocity.clear();
        old_velocity.clear();
        p_current.clear();
        BN_old_best.clear();
        BN_new_best.clear();

        part_1.clear();
        temp_part_2.clear();
        part_2.clear();
        temp_part_3.clear();
        part_3.clear();
    }
    //���¼��������ӵ�BICֵ��ͬʱ���¸����ӵ�����BICֵ��λ��
    for(int i=0; i<num_of_particles; i++)
    {
        current_BIC[i] = current_particles_network[i].ComputeBIC();//�˴���ComputeBIC����Ϊ��Particle�ĺ���
        if(ComputeBIC(current_BIC[i])>ComputeBIC(best_BIC[i]))
        {
            vector<vector<int> > temp_part2 = current_particles_network[i].GetPart2();
            best_particles_network[i].SetPart2(temp_part2);
            best_BIC[i] = current_BIC[i];
            temp_part2.clear();
        }
    }
    //�Ƚ����������е�BICֵ������ȫ���������ӵ�BICֵ��λ��
    for(int i=0; i<num_of_particles; i++)
    {
        if(ComputeBIC(best_BIC[i])>ComputeBIC(global_best_BIC))
        {
            vector<vector<int> > temp_part2 = best_particles_network[i].GetPart2();
            best_particle.SetPart2(temp_part2);
            global_best_BIC = best_BIC[i];
            temp_part2.clear();
        }
    }
    //�������º�����Ϣ
    for(int i=0; i<num_of_particles; i++)
    {
        //cout<<"Particle["<<i+1<<"]:"<<endl;
        //cout<<"Current network:";
        //current_particles_network[i].PrintParticle();
        //cout<<"Current BIC value = "<<ComputeBIC(current_BIC[i])<<endl;
        //PrintVelocity(i);
        //cout<<"Best network:";
        //best_particles_network[i].PrintParticle();
        //cout<<"Best BIC value = "<<ComputeBIC(best_BIC[i])<<endl<<endl;

        datalog<<"Particle["<<i+1<<"]:"<<endl;
        datalog<<"Current network:";
        current_particles_network[i].PrintLogToFile();
        datalog<<"Current BIC value = "<<ComputeBIC(current_BIC[i])<<endl;
        PrintVelocityToFile(i);
        datalog<<"Best network:";
        best_particles_network[i].PrintLogToFile();
        datalog<<"Best BIC value = "<<ComputeBIC(best_BIC[i])<<endl<<endl;
    }
    cout<<endl<<"Global best network:";
    best_particle.PrintParticle();
    cout<<"Global best BIC value = "<<ComputeBIC(global_best_BIC)<<endl<<endl<<endl;

    datalog<<"Global best network:";
    best_particle.PrintLogToFile();
    datalog<<"Global best BIC value = "<<ComputeBIC(global_best_BIC)<<endl<<endl<<endl;

    if(times==iterations)
    {
        netlog<<"Global best network:";
        best_particle.PrintNetworkToFile();
        netlog<<"Global best BIC value = "<<ComputeBIC(global_best_BIC)<<endl;
    }
}

//��Ҷ˹��������ѧϰ
void Network::ParameterLearning()
{
    probability_a.clear();
    probability_b.clear();
    vector<vector<int> > temp_part2 = best_particle.GetPart2();
    //�����������ʷֲ�
    cout<<"Conditional Probability Distribution:"<<endl<<endl;
    datalog<<"Conditional Probability Distribution:"<<endl<<endl;
    for(int i=0; i<num_of_nodes; i++)
    {
        cout<<"Node["<<i+1<<"]:"<<endl;
        datalog<<"Node["<<i+1<<"]:"<<endl;
        int qi =0, ri = 0;//qi���ڵ���ȡֵ״̬����ri�ӽڵ���ȡֵ״̬��
        vector<vector<int> > data_number;//�ӽڵ�i��ri��ȡֵ״̬�����ڵ㼯��qi��ȡֵ״̬����������
        ri = arranged_data[i].size();//��ȡ�ӽڵ�ȡֵ��
        qi = 1;
        if(temp_part2[i]!=zero)//�ڵ�i�и��ڵ�
        {
            vector<vector<float> > sequence;//���ڵ�ȡֵ����
            vector<int> parent_node_number;//��¼���ڵ㼯����
            vector<int> parent_node_value_range;//��¼���ڵ㼯ȡֵ����
            for(int j=0; j<max_parents; j++)
            {
                int temp_parent = temp_part2[i][j];
                if(temp_parent!=0)//������Ϊ0��Ϊ���ڵ�����
                {
                    int temp_size = arranged_data[temp_parent-1].size();//��ȡĳ�����ڵ���ȡֵ��
                    parent_node_number.push_back(temp_parent);//˳�����븸�ڵ�����
                    parent_node_value_range.push_back(temp_size);//˳�����븸�ڵ�ȡֵ��
                    qi *= temp_size;
                }
            }
            for(int m=0; m<ri; m++)//����ѭ��Ϊ�ӽڵ�ȡֵ��
            {
                vector<int> number;//�ڵ�iȡ��m��ֵʱ�����ڵ㼯ȡ��n������ֵ����������
                for(int n=0; n<qi; n++)//�ڲ�ѭ��Ϊ���ڵ㼯ȡֵ��
                {
                    int Nijk = 0;
                    float son_node_value = arranged_data[i][m];//��ȡ�ӽڵ���m��ֵ
                    vector<float> parent_node_values;//��¼���ڵ㼯��ȡֵ����
                    int shang = n, yushu = 0, temp_size = 0;
                    for(int j=0; j<parent_node_number.size(); j++)
                    {
                        temp_size = parent_node_value_range[j];//��ȡĳ�����ڵ���ȡֵ��
                        int temp_parent = parent_node_number[j];
                        yushu = shang%temp_size;//��ǰ���ڵ�ȡ��yushu��ֵ
                        parent_node_values.push_back(arranged_data[temp_parent-1][yushu]);
                        shang = shang/temp_size;
                    }
                    for(int p=0; p<data.size(); p++)
                    {
                        if(son_node_value == data[p][i])//�ӽڵ�ƥ����
                        {
                            int j;
                            for(j=0; j<parent_node_number.size(); j++)//ƥ�丸�ڵ�
                            {
                                int temp_parent = parent_node_number[j];//��ǰ���ڵ�����
                                if(parent_node_values[j] != data[p][temp_parent-1])
                                {
                                    break;
                                }
                            }
                            if(j == parent_node_number.size())
                            {
                                Nijk++;
                            }
                        }
                    }
                    number.push_back(Nijk);
                    sequence.push_back(parent_node_values);//
                    parent_node_values.clear();
                }
                data_number.push_back(number);
                number.clear();
            }
            int Nij = 0;//���ݼ��о��е�i�������ĸ��ڵ��ĵ�j��ֵ���ϵ�������
            vector<int> Nij_vector;
            for(int z=0; z<qi; z++)
            {
                for(int y=0; y<ri; y++)
                {
                    Nij += data_number[y][z];
                }
                Nij_vector.push_back(Nij);
                Nij = 0;
            }
            vector<vector<float> > probability;
            for(int k=0; k<qi; k++)//�����ڵ�ȡֵ�̶�ʱ
            {
                vector<float> temp_probability;//�����ڵ�ȡֵ�̶�,�ӽڵ�ȡֵ�仯ʱ��ri�ָ���
                for(int c=0; c<ri; c++)//�ӽڵ�ȡֵ�仯
                {
                    float tp;
                    if(Nij_vector[k]==0)
                    {
                        tp = 0;
                    }
                    else
                    {
                        tp = float(data_number[c][k])/float(Nij_vector[k]);
                    }
                    temp_probability.push_back(tp);
                }
                probability.push_back(temp_probability);
                temp_probability.clear();
            }
            for(int k=0; k<qi; k++)
            {
                for(int q=0; q<ri; q++)
                {
                    cout<<"P(Node["<<i+1<<"]="<<arranged_data[i][q]<<"|";
                    datalog<<"P(Node["<<i+1<<"]="<<arranged_data[i][q]<<"|";
                    for(int p=0; p<parent_node_number.size(); p++)
                    {
                        cout<<"Node["<<parent_node_number[p]<<"]="<<sequence[k][p];
                        datalog<<"Node["<<parent_node_number[p]<<"]="<<sequence[k][p];
                        if(p<parent_node_number.size()-1)
                        {
                            cout<<",";
                            datalog<<",";
                        }
                    }
                    cout<<")="<<probability[k][q]<<endl;
                    datalog<<")="<<probability[k][q]<<endl;
                }
            }
            probability_b.push_back(probability);
            Nij_vector.clear();
            probability.clear();
            parent_node_number.clear();
            sequence.clear();
            parent_node_value_range.clear();//��¼���ڵ㼯ȡֵ����
        }
        else//�ڵ�i�޸��ڵ�
        {
            vector<float> temp_probability;
            for(int k=0; k<ri; k++)
            {
                int cp = 0;
                float value = arranged_data[i][k];
                for(int q=0; q<data.size(); q++)
                {
                    if(data[q][i]==value)
                    {
                        cp++;
                    }
                }
                float pro = float(cp)/float(data.size());
                cout<<"P(Node["<<i+1<<"]="<<value<<")="<<pro<<endl;
                datalog<<"P(Node["<<i+1<<"]="<<value<<")="<<pro<<endl;
                temp_probability.push_back(pro);
            }
            probability_a.push_back(temp_probability);
            temp_probability.clear();
        }
        data_number.clear();
        cout<<endl;
        datalog<<endl;
    }
    cout<<endl;
    datalog<<endl;
    //������Ե���ʷֲ�
    cout<<"Marginal Probability Distribution:"<<endl<<endl;
    datalog<<"Marginal Probability Distribution:"<<endl<<endl;
    for(int i=0; i<num_of_nodes; i++)
    {
        cout<<"Node["<<i+1<<"]:"<<endl;
        datalog<<"Node["<<i+1<<"]:"<<endl;
        netlog<<"Node["<<i+1<<"]:"<<endl;
        int ri = arranged_data[i].size();//��ȡ�ӽڵ�ȡֵ��
        for(int k=0; k<ri; k++)
        {
            int cp = 0;
            float value = arranged_data[i][k];
            for(int q=0; q<data.size(); q++)
            {
                if(data[q][i]==value)
                {
                    cp++;
                }
            }
            float pro = float(cp)/float(data.size());
            cout<<"P(Node["<<i+1<<"]="<<value<<")="<<pro<<endl;
            datalog<<"P(Node["<<i+1<<"]="<<value<<")="<<pro<<endl;
            netlog<<"P(Node["<<i+1<<"]="<<value<<")="<<pro<<endl;
        }
    }
    cout<<endl;
    datalog<<endl;
    netlog<<endl;
    temp_part2.clear();
}

void Network::PrintVelocity(const int& particle_seq)
{
    cout<<"Velocity["<<particle_seq+1<<"]:";
    cout<<"<";
    for(int k=0; k<num_of_nodes; k++)
    {
        cout<<k+1;
        if(k!=num_of_nodes-1)
        {
            cout<<",";
        }
    }
    cout<<"|";
    for(int k=0; k<num_of_nodes; k++)
    {
        for(int j=0; j<max_parents; j++)
        {
            cout<<velocity[particle_seq][k][j];
            if(j!=max_parents-1)
            {
                cout<<" ";
            }
        }
        if(k!=num_of_nodes-1)
        {
            cout<<",";
        }
    }
    cout<<">"<<endl;
}

void Network::PrintVelocityToFile(const int& particle_seq)
{
    datalog<<"Velocity["<<particle_seq+1<<"]:";
    datalog<<"<";
    for(int k=0; k<num_of_nodes; k++)
    {
        datalog<<k+1;
        if(k!=num_of_nodes-1)
        {
            datalog<<",";
        }
    }
    datalog<<"|";
    for(int k=0; k<num_of_nodes; k++)
    {
        for(int j=0; j<max_parents; j++)
        {
            datalog<<velocity[particle_seq][k][j];
            if(j!=max_parents-1)
            {
                datalog<<" ";
            }
        }
        if(k!=num_of_nodes-1)
        {
            datalog<<",";
        }
    }
    datalog<<">"<<endl;
}

//����λ������һ������λ��������
vector<vector<int> > Network::PositionMinusPosition(const vector<vector<int> >& position_a, const vector<float>& BIC_a, const vector<float>& BIC_b)
{
    vector<vector<int> > temp;
    for(int i=0; i<num_of_nodes; i++)
    {
        if(BIC_a[i]>=BIC_b[i])
        {
            temp.push_back(position_a[i]);
        }
        else
        {
            temp.push_back(zero);
        }
    }
    return temp;
}

//�������ٶ�����
vector<vector<int> > Network::ConstMultiplyVelocity(const float& number_a, const vector<vector<int> >& velocity_b)
{
    srand((unsigned)time(NULL));
    vector<vector<int> > temp;
    if(number_a>=1)
    {
        temp = velocity_b;
        return temp;
    }
    else
    {
        for(int i=0; i<num_of_nodes; i++)
        {
            temp.push_back(zero);
        }
        vector<int> velo_pos;//�����ٶȷ����Ľڵ��±�
        for(int i=0; i<num_of_nodes; i++)
        {
            if(velocity_b[i]!=zero)
            {
                velo_pos.push_back(i);
            }
        }
        int ct = velo_pos.size();
        int num = floor(number_a * ct);
        if(num>0)
        {
            int p1, p2, tmp;
            while(--ct)
            {
                p1 = ct;
                p2 = rand()%ct;
                tmp = velo_pos[p1];
                velo_pos[p1] = velo_pos[p2];
                velo_pos[p2] = tmp;
            }
            int pos;
            for(int i=0; i<num; i++)
            {
                pos = velo_pos[i];
                temp[pos] = velocity_b[pos];
            }
        }
        velo_pos.clear();
    }
    return temp;
}

//�ٶ����ٶ�����
vector<vector<int> > Network::VelocityPlusVelocity(const vector<vector<int> >& velocity_a, const vector<vector<int> >& velocity_b)
{
    vector<vector<int> > temp;
    srand((unsigned)time(NULL));
    for(int i=0; i<num_of_nodes; i++)
    {
        if(velocity_a[i]!=zero&&velocity_b[i]==zero)
        {
            temp.push_back(velocity_a[i]);
        }
        else if(velocity_a[i]==zero&&velocity_b[i]!=zero)
        {
            temp.push_back(velocity_b[i]);
        }
        else
        {
            int pos = rand()%2;
            if(pos==0)
            {
                temp.push_back(velocity_a[i]);
            }
            if(pos==1)
            {
                temp.push_back(velocity_b[i]);
            }
        }
    }
    return temp;
}

//λ�����ٶ�����
vector<vector<int> > Network::PositionPlusVelocity(const vector<vector<int> >& position_a, const vector<vector<int> >& velocity_b)
{
    vector<vector<int> > temp;
    for(int i=0; i<num_of_nodes; i++)
    {
        if(velocity_b[i]!=zero)
        {
            temp.push_back(velocity_b[i]);
        }
        else
        {
            temp.push_back(position_a[i]);
        }
    }
    return temp;
}

//����һ����Ҷ˹�������ڵ���BICֵ�ܺ�
float Network::ComputeBIC(const vector<float>& temp_BIC)
{
    float total_BIC = 0;
    for(int i=0; i<temp_BIC.size(); i++)
    {
        total_BIC += temp_BIC[i];
    }
    return total_BIC;
}

//����Ȩ�����Ӻ���
float Network::WFunction(const int& times2)
{
    float w_start = 0.9, w_end = 0.4, result;
    result = w_start - (w_start - w_end) * (times2/iterations) * (times2/iterations);
    return result;
}

//�������Ӻ���
float Network::QFunction(const int& q1, const int& q2)
{
    float q = q1 + q2, result;
    result = 2/abs(2-q-sqrt(q*q-4*q));
    return result;
}

int main()
{
    int dataset, num_of_particles, max_parents, iterations;
    cout<<"Please select dataset: 1.Asia; 2.Others"<<endl;
    cout<<"dataset=";
    cin>>dataset;
    while(dataset!=1&&dataset!=2)
    {
        cout<<"Please select dataset: 1.Asia; 2.Others"<<endl;
        cout<<"dataset=";
        cin>>dataset;
    }
    cout<<"num_of_particles=";
    cin>>num_of_particles;
    cout<<"max_parents=";
    cin>>max_parents;
    cout<<"iterations=";
    cin>>iterations;
    cout<<endl;
    Network network = Network(dataset, num_of_particles, max_parents, iterations);
    network.NetworkLearning();
    return 0;
}
