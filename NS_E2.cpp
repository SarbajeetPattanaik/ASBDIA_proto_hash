#include<iostream>
#include<fstream>
#include<sstream>
#include<cryptopp/gf256.h>
#include<time.h>
#include<iomanip>
#include<bitset>

using namespace std;
using namespace CryptoPP;

vector<GF256::Element> vec_mul(GF256::Element vec1,char buffer[],int m);
vector<GF256::Element> vec_mul2(GF256::Element vec1,vector<GF256::Element> buffer,int k);
vector<GF256::Element> MACSIG(char buffer[],vector<int> alp1,int m) {  
    int ic=0;
    vector<GF256::Element> mac1;
    int k=alp1.size();
    for(int i=0;i<k;i++){
        ic=0;
        int alp;
        alp=alp1[i];
        GF256 gf256(283); 
        GF256::Element a; 
        GF256::Element key = alp;
        GF256::Element b = key;
        GF256::Element mac = 0;
	
        char b1;

        while (ic < m) {
                b1 = buffer[ic];
                int num = int(b1);
                //cout <<b1<<" "<<num;
                a = num;
                a = gf256.Multiply(a,b); 

                mac = gf256.Add(a,mac); 
                b = gf256.Multiply(b,key);
                ic++;
        }
        mac1.push_back(mac);
        //stringstream res;
        //bitset<8> mac13 (mac);
        //cout<<mac13<<endl;
        //res << hex << setfill('0') << setw(2) << mac13.to_ulong();
        //cout<<res.str()<<" ";
    }
    return mac1;
}

vector<GF256::Element> MACSIG(vector<GF256::Element> buffer,vector<int> alp1,int m) {  
    int ic=0;
    vector<GF256::Element> mac1;
    int k=alp1.size();
    for(int i=0;i<k;i++){
        ic=0;
        int alp;
        alp=alp1[i];
        GF256 gf256(283); 
        GF256::Element a; 
        GF256::Element key = alp;
        GF256::Element b = key;
        GF256::Element mac = 0;
	
        //char b1;

        while (ic < m) {
                a = buffer[ic];
                a = gf256.Multiply(a,b); 

                mac = gf256.Add(a,mac); 
                b = gf256.Multiply(b,key);
                ic++;
        }
        mac1.push_back(mac);
        //stringstream res;
        //bitset<8> mac13 (mac);
        //cout<<mac13<<endl;
        //res << hex << setfill('0') << setw(2) << mac13.to_ulong();
        //cout<<res.str()<<" ";
    }
    return mac1;
}
int main()
{

    int m;
    cout<<"m:";
    cin>>m;

    string filename;
    cout<<"file:";
    cin>>filename;
    std::ifstream file(filename, std::ios::binary);
	int p = 0;

	if (!file.is_open()) {
		std::cerr << "Error opening file" <<std::endl;
		exit(1);
	}
	file.seekg(0, std::ios::end);
    //file size;
    std::streampos fileSize = file.tellg();
    if (fileSize == -1) {
        std::cerr << "Error getting file size\n";
        return 1;
    }

    //std::cout << "File size is: " << fileSize << " bytes\n";
    file.seekg(0, std::ios::beg);
    int n=0;
    n=ceil(float(fileSize)/float(m));
    cout<<"n:"<<n<<endl;
    cout<<"m:"<<m<<endl;
    int k;
    cout<<"k:";
    cin>>k;
    vector<int> alp(k,0);
    cout<<"alp:";
    for(int i=0;i<k;i++){
        cin>>alp[i];
    }
    cout<<"vect:"<<endl;
    vector<vector<GF256::Element>> Tag;
    vector<GF256::Element> acc_block(m,0);
	vector<GF256::Element> vec1(n,0);
    //GF256::Element *vec1=(GF256::Element *)malloc(n*sizeof(GF256::Element));

    for(int i=0;i<n;i++){
        //int el;
        cin>>vec1[i];
        //vec1.push_back(el);
    }
    
    
    GF256 gf256(283); 
	GF256::Element a; 

    char b1;
	int ic = 0;
    char buffer[m]; // Buffer to store m bytes
    clock_t t1;
    clock_t st=0;
    cout<<"Tag file:"<<endl;
    while (file.read(buffer, m) || file.gcount() > 0) { 
        vector<GF256::Element> mac_1;
        mac_1=MACSIG(buffer,alp,m);

        vector<GF256::Element> temp1(m,0);
        t1 = clock();
        temp1=vec_mul(vec1[ic],buffer,m);
        t1 = clock() - t1;
        st+=t1;
        

        for (int i = 0; i < m; ++i) 
        {
            acc_block[i] = gf256.Add(temp1[i], acc_block[i]);
        }
        
        memset(buffer, 0, sizeof(buffer));
        Tag.push_back(mac_1);
        for(int j=0;j<k;j++){
            stringstream res;
            GF256::Element mac134=Tag[ic][j];
            bitset<8> mac13 (mac134);
            //cout<<mac13<<endl;
            res << hex << setfill('0') << setw(2) << mac13.to_ulong();
            cout<<res.str()<<" ";
        }
        ic++;
        cout<<endl;
    }
    
    //Tag file
    /*
    cout<<"Tag file:"<<endl;
    for(int i=0;i<n;i++){
        for(int j=0;j<k;j++){
            stringstream res;
            GF256::Element mac134=Tag[i][j];
            bitset<8> mac13 (mac134);
            //cout<<mac13<<endl;
            res << hex << setfill('0') << setw(2) << mac13.to_ulong();
            cout<<res.str()<<" ";
        }
        cout<<endl;
    }
    */

    //print u
    cout<<"EXP 1"<<endl;
    cout<<"µ value(hex):";
    for(int i=0;i<m;i++){
        stringstream res;
            GF256::Element temp=acc_block[i];
            bitset<8> temp1 (temp);
            //cout<<mac13<<endl;
            res << hex << setfill('0') << setw(2) << temp1.to_ulong();
            cout<<res.str()<<"  ";
    }
    cout<<endl<<"EXP 2"<<endl;
    vector<GF256::Element> t(k,0);
    
    ic=0;
    while (ic<n) {
        vector<GF256::Element> temp1(k,0);
        vector<GF256::Element> buff1;
        for(int i=0;i<k;i++){
            buff1.push_back(Tag[ic][i]);
        }
        temp1=vec_mul2(vec1[ic],buff1,k);

        for (int i = 0; i < k; ++i) 
        {
            t[i] = gf256.Add(temp1[i], t[i]);
        }
        ic++;
    }

    cout<<"τ:";
    for(int i=0;i<k;i++){
        stringstream res;
            GF256::Element temp=t[i];
            bitset<8> temp1 (temp);
            //cout<<mac13<<endl;
            res << hex << setfill('0') << setw(2) << temp1.to_ulong();
            cout<<res.str()<<"  ";
    }


    cout<<endl<<"EXP 3"<<endl;
    vector<GF256::Element> mac_2;
    mac_2=MACSIG(acc_block,alp,m);
    cout<<"MACSIG(µ, α):";
    for(int i=0;i<k;i++){
        stringstream res;
            GF256::Element temp=mac_2[i];
            bitset<8> temp1 (temp);
            //cout<<mac13<<endl;
            res << hex << setfill('0') << setw(2) << temp1.to_ulong();
            cout<<res.str()<<"  ";
    }












 
    //cout<<ic<<endl;
    //cout<<buffer<<endl;
    //cout<<file.gcount()<<endl;
    
    cout<<endl;
     // t = end time - start time
	cout << "Execution Time : " << (((double)st / CLOCKS_PER_SEC) ) <<endl;

    return 0;
}

vector<GF256::Element> vec_mul(GF256::Element vec1,char buffer[],int m)
{
    GF256 gf256(283);
    vector<GF256::Element> temp1(m,0);
    for(int i=0;i<m;i++){

        GF256::Element a=static_cast<GF256::Element>(buffer[i]);
        temp1[i]=gf256.Multiply(vec1,a);
    }
    return temp1;
}

vector<GF256::Element> vec_mul2(GF256::Element vec1,vector<GF256::Element> buffer,int k)
{
    GF256 gf256(283);
    vector<GF256::Element> temp1(k,0);
    for(int i=0;i<k;i++){

        GF256::Element a=buffer[i];
        temp1[i]=gf256.Multiply(vec1,a);
    }
    return temp1;
}
