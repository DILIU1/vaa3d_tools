#ifndef __MY_SURF_OBJS_CPP__
#define __MY_SURF_OBJS_CPP__
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>

#include "basic_surf_objs.h" //added by PHC, 2013-01-03

#include "my_surf_objs.h"

#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

using namespace std;

vector<MyMarker> readMarker_file(string marker_file)
{
	vector<MyMarker> markers; markers.reserve(1000);
	ifstream ifs(marker_file.c_str()); if(ifs.fail()){cout<<" unable to open marker file "<<marker_file<<endl; return markers;}
	set<MyMarker> marker_set;
	int count = 0;
	while(ifs.good())
	{
		if(ifs.peek() == '#' || ifs.eof()){ifs.ignore(1000,'\n'); continue;}
		MyMarker marker;
		ifs>>marker.x;ifs.ignore(10,',');ifs>>marker.y;ifs.ignore(10,',');ifs>>marker.z;ifs.ignore(10,',');ifs>>marker.radius;ifs.ignore(1000,'\n');
		
		marker.x -= MARKER_BASE;
		marker.y -= MARKER_BASE;
		marker.z -= MARKER_BASE;

		if(0 && marker_set.find(marker) != marker_set.end())
		{
			cout<<"omit duplicated marker"<<markers.size()<<" : x = "<<marker.x<<" y = "<<marker.y<<" z = "<<marker.z<<" r = "<<marker.radius<<endl;
		}
		else 
		{
			markers.push_back(marker);
			marker_set.insert(marker);
			if(0) cout<<"marker"<<markers.size()<<" : x = "<<marker.x<<" y = "<<marker.y<<" z = "<<marker.z<<" r = "<<marker.radius<<endl;
		}
		count++;
	}
	cout<<count<<" markers loaded"<<endl;
	ifs.close();
	return markers;
}

bool readMarker_file(string marker_file, vector<MyMarker*> &markers)
{
	ifstream ifs(marker_file.c_str()); if(ifs.fail()){cout<<" unable to open marker file "<<marker_file<<endl; return false;}
	set<MyMarker> marker_set; // to check the duplicated marker
	while(ifs.good())
	{
		if(ifs.peek() == '#' || ifs.eof()){ifs.ignore(1000,'\n'); continue;}
		MyMarker *marker = new MyMarker;
		ifs>>marker->x;ifs.ignore(10,',');ifs>>marker->y;ifs.ignore(10,',');ifs>>marker->z;ifs.ignore(10,',');ifs>>marker->radius;ifs.ignore(1000,'\n');

		marker->x -= MARKER_BASE;
		marker->y -= MARKER_BASE;
		marker->z -= MARKER_BASE;

		if(marker_set.find(*marker) != marker_set.end())
		{
			cout<<"omit duplicated marker"<<markers.size()<<" : x = "<<marker->x<<" y = "<<marker->y<<" z = "<<marker->z<<" r = "<<marker->radius<<endl;
		}
		else
		{
			markers.push_back(marker);
			marker_set.insert(*marker);
			if(0) cout<<"marker"<<markers.size()<<" : x = "<<marker->x<<" y = "<<marker->y<<" z = "<<marker->z<<" r = "<<marker->radius<<endl;
		}
	}
	ifs.close();
	return true;
}

bool saveMarker_file(string marker_file, vector<MyMarker> & outmarkers)
{
    list<string> nullinfostr;
    return saveMarker_file(marker_file, outmarkers, nullinfostr);
}

bool saveMarker_file(string marker_file, vector<MyMarker> & outmarkers, list<string> & infostring)
{
	cout<<"save "<<outmarkers.size()<<" markers to file "<<marker_file<<endl;
	ofstream ofs(marker_file.c_str()); 
	
	if(ofs.fail())
	{
		cout<<"open marker file error"<<endl; 
		return false;
	}
    
    list<string>::iterator it;
    for (it=infostring.begin();it!=infostring.end(); it++)
        ofs<< *it <<endl;
    
	ofs<<"#x, y, z, radius"<<endl;
	for(int i = 0; i < outmarkers.size(); i++)
	{
		ofs<<outmarkers[i].x + MARKER_BASE<<","<<outmarkers[i].y + MARKER_BASE<<","<<outmarkers[i].z + MARKER_BASE<<","<<outmarkers[i].radius<<endl;
	}
	ofs.close();
	return true;
}

bool saveMarker_file(string marker_file, vector<MyMarker*> & outmarkers)
{
    list<string> nullinfostr;
    return saveMarker_file(marker_file, outmarkers, nullinfostr);    
}

bool saveMarker_file(string marker_file, vector<MyMarker*> & outmarkers, list<string> & infostring)
{
	cout<<"save "<<outmarkers.size()<<" markers to file "<<marker_file<<endl;
	ofstream ofs(marker_file.c_str()); 
	
	if(ofs.fail())
	{
		cout<<"open marker file error"<<endl; 
		return false;
	}
    
    list<string>::iterator it;
    for (it=infostring.begin();it!=infostring.end(); it++)
        ofs<< *it <<endl;

	ofs<<"#x, y, z, radius"<<endl;
	for(int i = 0; i < outmarkers.size(); i++)
	{
		ofs<<outmarkers[i]->x + MARKER_BASE<<","<<outmarkers[i]->y + MARKER_BASE<<","<<outmarkers[i]->z+MARKER_BASE<<","<<outmarkers[i]->radius<<endl;
	}
	ofs.close();
	return true;
}

vector<MyMarker*> readSWC_file(string swc_file)
{
	vector<MyMarker*> swc;

	ifstream ifs(swc_file.c_str());

	if(ifs.fail()) 
	{
		cout<<"open swc file : "<< swc_file <<" error"<<endl;
		return swc;
	}

	map<int, MyMarker*> marker_map;
	map<MyMarker*, int> parid_map;
	while(ifs.good())
	{
		if(ifs.peek() == '#'){ifs.ignore(1000,'\n'); continue;}
		MyMarker *  marker = new MyMarker;
		int my_id = -1 ; ifs >> my_id;
		if(my_id == -1) break;
		if(marker_map.find(my_id) != marker_map.end())
		{
            cerr<<"Duplicate Node. This is a graph file. Please read is as a graph."<<endl; //return vector<MyMarker*>();
		}
		marker_map[my_id] = marker;

		ifs>> marker->type;
		ifs>> marker->x;
		ifs>> marker->y;
		ifs>> marker->z;
		ifs>> marker->radius;
		int par_id = -1; ifs >> par_id;

		parid_map[marker] = par_id;
		swc.push_back(marker);
	}
	ifs.close();
	vector<MyMarker*>::iterator it = swc.begin();
	while(it != swc.end())
	{
		MyMarker * marker = *it;
		marker->parent = marker_map[parid_map[marker]];
		it++;
	}
	return swc;
}

bool readSWC_file(string swc_file, vector<MyMarker> & outmarkers)
{
    ifstream ifs(swc_file.c_str());

    if(ifs.fail())
    {
        cout<<"open swc file : "<< swc_file <<" error"<<endl;
        return false;
    }

    while(ifs.good())
    {
        if(ifs.peek() == '#'){ifs.ignore(1000,'\n'); continue;}
        MyMarker  marker;
        int my_id = -1 ; ifs >> my_id;
        if(my_id == -1) break;

        ifs>> marker.type;
        ifs>> marker.x;
        ifs>> marker.y;
        ifs>> marker.z;
        ifs>> marker.radius;
        int par_id = -1; ifs >> par_id;
        //cout<<"("<<marker.x<<","<<marker.y<<","<<marker.z<<")"<<endl;

        outmarkers.push_back(marker);
    }
    ifs.close();
    return true;
}

bool readSWC_file(string swc_file, vector<MyMarker*> & swc)
{//debug by guochanghao, fix for reading failure after 7 values
    ifstream ifs(swc_file.c_str());

    if(ifs.fail())
    {
        cout<<"open swc file : "<< swc_file <<" error"<<endl;
        return false;
    }

    map<int, MyMarker*> marker_map;
    map<MyMarker*, int> parid_map;
    string line;
    float ave_count_each_line_num = 0.0;
    int num_valid_line = 0;
    while(getline(ifs, line))
    {
        int count_each_line_num = 0;
        stringstream ss(line);
        string tmp;
        bool skipToNextLine = false;

        while(getline(ss,tmp,' '))
        {
            if(tmp.at(0) == '#'){skipToNextLine=true; break;}
//            cout<<tmp<<" ";
            count_each_line_num++;
        }
        if(skipToNextLine){continue;}
        num_valid_line++;
        ave_count_each_line_num+=count_each_line_num;
//        cout<<line<<": "<<count_each_line_num<<endl;
    }
    ave_count_each_line_num/=num_valid_line;
    cout<<"ave_count_each_line_num:"<<ave_count_each_line_num<<endl;
    if(ave_count_each_line_num-(int)ave_count_each_line_num==0)
    {qDebug("Each line has the same number of parameters."); ifs.close();}
    else
    {qDebug("ave_count_each_line_num is not integer."); ifs.close(); return false;}


    ifs.open(swc_file.c_str());
    while(ifs.good())
    {
        if(ifs.peek() == '#'){ifs.ignore(1000,'\n'); continue;}
        MyMarker *  marker = new MyMarker;
        int my_id = -1 ; ifs >> my_id;
        if(my_id == -1) break;
        if(marker_map.find(my_id) != marker_map.end())
        {
            cerr<<"Duplicate Node. This is a graph file. Please read it as a graph."<<endl; return false;
        }
        marker_map[my_id] = marker;

        ifs>> marker->type;
        ifs>> marker->x;
        ifs>> marker->y;
        ifs>> marker->z;
        ifs>> marker->radius;

        int par_id = -1; ifs >> par_id;
        parid_map[marker] = par_id;
        if(1)
        {//readSWC_file这个函数直接读swc文件有问题，后面seg_id等不读。跳过。
            int temp_for_;
            for(int i=ave_count_each_line_num-7; i>0; i--)
            {
                ifs>>temp_for_;
            }
        }
        swc.push_back(marker);
    }


    ifs.close();
    vector<MyMarker*>::iterator it = swc.begin();
    while(it != swc.end())
    {
        MyMarker * marker = *it;
        marker->parent = marker_map[parid_map[marker]];
        it++;
    }
    return true;
}

bool saveSWC_file(string swc_file, vector<MyMarker*> & outmarkers)
{
    list<string> nullinfostr;
    return saveSWC_file(swc_file, outmarkers, nullinfostr);    
}

bool saveSWC_file(string swc_file, vector<MyMarker*> & outmarkers, list<string> & infostring)
{
	if(swc_file.find_last_of(".dot") == swc_file.size() - 1) return saveDot_file(swc_file, outmarkers);

	cout<<"marker num = "<<outmarkers.size()<<", save swc file to "<<swc_file<<endl;
	map<MyMarker*, int> ind;	
	ofstream ofs(swc_file.c_str()); 
	
	if(ofs.fail())
	{
		cout<<"open swc file error"<<endl; 
		return false;
	}
	ofs<<"#name "<<swc_file<<endl;
	ofs<<"#comment "<<endl;
    
    list<string>::iterator it;
    for (it=infostring.begin();it!=infostring.end(); it++)
        ofs<< *it <<endl;
    
	ofs<<"##n,type,x,y,z,radius,parent"<<endl;
	for(int i = 0; i < outmarkers.size(); i++) ind[outmarkers[i]] = i+1;

	for(int i = 0; i < outmarkers.size(); i++)
	{
		MyMarker * marker = outmarkers[i];
		int parent_id;
		if(marker->parent == 0) parent_id = -1;
		else parent_id = ind[marker->parent];
		ofs<<i+1<<" "<<marker->type<<" "<<marker->x<<" "<<marker->y<<" "<<marker->z<<" "<<marker->radius<<" "<<parent_id<<endl;
	}
	ofs.close();
	return true;
}

bool saveSWC_file(string swc_file, vector<NeuronSWC*> & outmarkers, list<string> & infostring)
{
	cout<<"marker num = "<<outmarkers.size()<<", save swc file to "<<swc_file<<endl;
	ofstream ofs(swc_file.c_str());
	
	if(ofs.fail())
	{
		cout<<"open swc file error"<<endl;
		return false;
	}
	ofs<<"#name "<<swc_file<<endl;
	ofs<<"#comment "<<endl;
    
    list<string>::iterator it;
    for (it=infostring.begin();it!=infostring.end(); it++)
        ofs<< *it <<endl;
    
	ofs<<"##n,type,x,y,z,radius,parent"<<endl;
    
	for(V3DLONG i = 0; i < outmarkers.size(); i++)
	{
		NeuronSWC * marker = outmarkers[i];
		int parent_id = (marker->parent < 0) ? -1 : marker->parent;
		ofs<<i<<" "<<marker->type<<" "<<marker->x<<" "<<marker->y<<" "<<marker->z<<" "<<marker->radius<<" "<<parent_id<<endl;
	}
	ofs.close();
	return true;
}

bool saveDot_file(string dot_file, vector<MyMarker*> & outmarkers)
{
	cout<<"marker num = "<<outmarkers.size()<<", save swc file to "<<dot_file<<endl;
	map<MyMarker*, int> ind;	
	ofstream ofs(dot_file.c_str()); 
	
	if(ofs.fail())
	{
		cout<<"open swc file error"<<endl; 
		return false;
	}
	ofs<<"digraph \""<<dot_file<<"\" {"<<endl;
	ofs<<"\trankdir = BT;"<<endl;

	for(int i = 0; i < outmarkers.size(); i++) ind[outmarkers[i]] = i+1;
	for(int i = 0; i < outmarkers.size(); i++)
	{
		MyMarker * marker = outmarkers[i];
		if(marker->parent)
		{
			int parent_id = ind[marker->parent];
			MyMarker * parent = marker->parent;
			ofs<<"\t"<<i+1<<" -> "<<parent_id<<";"<<endl;
		}
	}
	ofs<<"}"<<endl;
	ofs.close();
	return true;
}

#ifdef __ESWC__
bool readESWC_file(string swc_file, vector<MyMarkerX*> & swc)
{//debug by guochanghao, fix for reading failure after 7 values
	ifstream ifs(swc_file.c_str());

	if(ifs.fail()) 
	{
        cout<<"open eswc file : "<< swc_file <<" error"<<endl;
		return false;
	}

	map<int, MyMarkerX*> marker_map;
	map<MyMarkerX*, int> parid_map;
    string line;
    float ave_count_each_line_num = 0.0;
    int num_valid_line = 0;
    while(getline(ifs, line))
    {
        int count_each_line_num = 0;
        stringstream ss(line);
        string tmp;
        bool skipToNextLine = false;

        while(getline(ss,tmp,' '))
        {
            if(tmp.at(0) == '#'){skipToNextLine=true; break;}
//            cout<<tmp<<" ";
            count_each_line_num++;
        }
        if(skipToNextLine){continue;}
        num_valid_line++;
        ave_count_each_line_num+=count_each_line_num;
//        cout<<line<<": "<<count_each_line_num<<endl;
    }
    ave_count_each_line_num/=num_valid_line;
    cout<<"ave_count_each_line_num:"<<ave_count_each_line_num<<endl;
    if(ave_count_each_line_num-(int)ave_count_each_line_num==0)
    {qDebug("Each line has the same number of parameters."); ifs.close();}
    else
    {qDebug("ave_count_each_line_num is not integer."); ifs.close(); return false;}


    ifs.open(swc_file.c_str());
    while(ifs.good())
    {
        if(ifs.peek() == '#'){ifs.ignore(1000,'\n'); continue;}
        MyMarkerX *  marker = new MyMarkerX;
        int my_id = -1 ; ifs >> my_id;
        if(my_id == -1) break;
        if(marker_map.find(my_id) != marker_map.end())
        {
            cerr<<"Duplicate Node. This is a graph file. Please read it as a graph."<<endl; return false;
        }
        marker_map[my_id] = marker;

        ifs>> marker->type;
        ifs>> marker->x;
        ifs>> marker->y;
        ifs>> marker->z;
        ifs>> marker->radius;
        marker->seg_id = -1;
        marker->seg_level = -1;
        marker->feature = 0.0;
        int par_id = -1; ifs >> par_id;
        parid_map[marker] = par_id;
        if(1)
        {//readESWC_file这个函数直接读eswc文件有问题，后面seg_id等不读。跳过。
            int temp_for_;
            for(int i=ave_count_each_line_num-7; i>0; i--)
            {
                ifs>>temp_for_;
            }
        }
        swc.push_back(marker);
    }


	ifs.close();
	vector<MyMarkerX*>::iterator it = swc.begin();
	while(it != swc.end())
	{
		MyMarkerX * marker = *it;
		marker->parent = marker_map[parid_map[marker]];
		it++;
	}
	return true;
}

bool saveESWC_file(string swc_file, vector<MyMarkerX*> & outmarkers)
{
    list<string> nullinfostr;
    return saveESWC_file(swc_file, outmarkers, nullinfostr);        
}

bool saveESWC_file(string swc_file, vector<MyMarkerX*> & outmarkers, list<string> & infostring)
{
	cout<<"marker num = "<<outmarkers.size()<<", save swc file to "<<swc_file<<endl;
	map<MyMarkerX*, int> ind;	
	ofstream ofs(swc_file.c_str()); 
	
	if(ofs.fail())
	{
		cout<<"open swc file error"<<endl; 
		return false;
	}
	ofs<<"#name "<<swc_file<<endl;
	ofs<<"#comment "<<endl;
    
    list<string>::iterator it;
    for (it=infostring.begin();it!=infostring.end(); it++)
        ofs<< *it <<endl;
    
	ofs<<"##n,type,x,y,z,radius,parent,seg_id,seg_level,feature"<<endl;
	for(int i = 0; i < outmarkers.size(); i++) ind[outmarkers[i]] = i+1;

	for(int i = 0; i < outmarkers.size(); i++)
	{
		MyMarkerX * marker = outmarkers[i];
		int parent_id;
		if(marker->parent == 0) parent_id = -1;
		else parent_id = ind[(MyMarkerX *) marker->parent];
		ofs<<i+1<<" "<<marker->type<<" "<<marker->x<<" "<<marker->y<<" "<<marker->z<<" "<<marker->radius<<" "<<parent_id<<" "<<marker->seg_id<<" "<<marker->seg_level<<" "<<marker->feature<<endl;
	}
	ofs.close();
	return true;
}
#endif

double dist(MyMarker a, MyMarker b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}

vector<MyMarker*> getLeaf_markers(vector<MyMarker*> & inmarkers)
{
	set<MyMarker*> par_markers;
	vector<MyMarker*> leaf_markers;
	for(int i = 0; i < inmarkers.size(); i++)
	{
		MyMarker * marker = inmarkers[i];
		if(marker->parent) par_markers.insert(marker->parent);
	}
	for(int i = 0; i < inmarkers.size(); i++)
	{
		if(par_markers.find(inmarkers[i]) == par_markers.end()) leaf_markers.push_back(inmarkers[i]);
	}
	par_markers.clear();
	return leaf_markers;
}

vector<MyMarker*> getLeaf_markers(vector<MyMarker*> & inmarkers, map<MyMarker *, int> & childs_num)
{
	for(int i = 0; i < inmarkers.size(); i++) childs_num[inmarkers[i]] = 0;

	vector<MyMarker*> leaf_markers;
	for(int i = 0; i < inmarkers.size(); i++)
	{
		MyMarker * marker = inmarkers[i];
		MyMarker * parent = marker->parent;
		if(parent) childs_num[parent]++;
	}
	for(int i = 0; i < inmarkers.size(); i++)
	{
		if(childs_num[inmarkers[i]] == 0) leaf_markers.push_back(inmarkers[i]);
	}
	return leaf_markers;
}
#endif
