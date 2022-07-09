#include <iostream>
#include <istream>
#include "rapidxml.hpp"
#include <vector>
#include "rapidxml_utils.hpp"
#include <bits/stdc++.h>
#include <cctype>

using namespace std;
using namespace rapidxml;


typedef struct
{
    /* data */
    string name;
    long long int id;
    double lat;
    double lon;

}NodeData;

vector<NodeData> nodes;
vector<pair<long double, NodeData>> list_of_distace;

void distances(int index);
bool compare(pair<long double, NodeData> node1, pair<long double, NodeData> node2);
void print_location(string location_name);

int main()
{   
    /////parsing through osm file using rapidxml library
    ifstream myFile("map.osm");
    xml_document<> doc;
    vector<char> buffer((istreambuf_iterator<char>(myFile)), istreambuf_iterator<char>());
     buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    xml_node<> *root_node = NULL;

    int no_of_nodes=0;
    int no_of_ways=0;
     
    root_node = doc.first_node("osm");

    string str1 = "node";
    string str2 = "way";
    string str3 = "name";

    
    vector<long long int> node_ids;

////////////reading all the nodes(node) in osm file in the struct vector nodes

    for(xml_node<> *Node = root_node->first_node("node"); Node; Node = Node->next_sibling())
    {
        if(Node->name() == str1) 
        {
            no_of_nodes++;///calculating number of nodes present
            NodeData newdata;
            newdata.id = stoll(Node->first_attribute("id")->value());
            newdata.lat = stod(Node->first_attribute("lat")->value());
            newdata.lon = stod(Node->first_attribute("lon")->value());
            for(xml_node<> *tag_node = Node->first_node("tag");tag_node;tag_node=tag_node->next_sibling())
            {
                if(tag_node->first_attribute("k")->value() == str3)
                {
                    newdata.name = tag_node->first_attribute("v")->value();
                }
            }
            nodes.push_back(newdata);///adding the newnode to the vector nodes.
            node_ids.push_back(newdata.id);

        }
        if(Node->name() == str2) no_of_ways++;////calculating number od ways

    } 
/////////ends

cout <<"\nNumber of nodes: " << no_of_nodes << endl;
cout <<"\nNumber of ways:  " << no_of_ways << endl;///printing number of nodes and ways


 /////////////
////////////////
///////////////part1(searching for location)
///////////////

int search;
cout<<"\n\nSEARCH LOCATION"<<" \nenter 1 to proceed"<<endl;
cin>>search;

while(search == 1)
{
//cout <<"\n\n\nSEARCH FOR LOCATION:"<<endl;
cout<<"\n\nName: "<<endl;
string location_name;
cin >> location_name;

print_location(location_name);//calling print location fuction

cout<<"\n\nwant to search Again?->ENTER 1\t Else enter any other integer"<<endl;
cin>>search;

}

/////////////////
/////////////////part 2 (finding k closest node)
////////////////

cout<<"\n\n\nEnter any positive integer to proceed to search for closest nodes to Your location"<<endl;

    int K;
    cin>>K;
    while(K > 0)
    {
cout<<"\n\tDON'T know the id of your location?\n\nDon't Worry press 1 to search for ID by name of your location"<<endl;
      int dont_know;
      cin>>dont_know;

      if(dont_know == 1)
      {   
          cout<< "\nName of your location"<<endl;
          string str4;
          cin>>str4;
          print_location(str4);
      }

cout<<"\n\n\nEnter the id of your location: "<<endl;
    long long int your_id;
    cin>> your_id;

  
      int index = -1;
      for(int i=0;i<nodes.size();i++)
      {
          if(nodes[i].id == your_id)
          {
              index = i;
              break;
          }
      }
      if(index == -1)
      {
          cout<<"\n\nWARNING: No node with the given id found"<<endl;
          cout<<"\n\nwant to continue->press 1"<<endl;
          int cont;
          cin>>cont;
          if(cont == 1)continue;
          else break;
      }

    int k;
      cout<<"\n\nEnter the number of closest node you want to find: "<<endl;
      cin>>k;
      if(k<0 && k>nodes.size())
      {   
          cout << "\nEnter a valid number" <<endl;
          continue;
      }

      distances(index);

      sort(list_of_distace.begin(), list_of_distace.end(),compare);

      cout << k <<" closest locations from your location id: " <<your_id<<endl;
      for(int j=0;j<k;j++)
      {
        cout<< j+1 <<"\n"<< "name: "<<list_of_distace[j].second.name <<"\nid: "<<list_of_distace[j].second.id<<endl;
        cout<<"distace: "<<list_of_distace[j].first<<endl;

      }

      
        /////list_of_distace.clear();//erase all the element in vector list_of_distance.
        cout<<list_of_distace.size();

      int choice;
      cout<<"press 1 to continue \t press any other number to exit"<<endl;
      cin>>choice;
      if(choice != 1) K=0;

    }
    ////////////////////////////////
    /////////***end of part 2/////
    /////////////////////////////


    //////////////////////////////////////////////*/

    return 0;
}

////printing location based on search string
void print_location(string location_name)
{
  int n;
  n = location_name.size();
  /////cout<<"\n"<<n<<endl;
  int m = nodes.size();
  ///cout <<"\n"<<m<<endl;
  int number_of_location_found;

  for(int i=0; i<m ;i++)
  {   
      int l= nodes[i].name.size();
      if(l >= n)
      {
          for(int j=0;j<=(l-n);j++)
          {   
              int count = 0;
              for(int k=0;k<n;k++)
               {
                   if(tolower(nodes[i].name.substr(j,n)[k]) == tolower(location_name[k]))count++;
               }
               
               if(count == n)
               {
                   cout<<"\n\nName: "<<nodes[i].name<<"\tID: "<<nodes[i].id<<"\tlon: "<<nodes[i].lon<<"\tlat: "<<nodes[i].lat<<endl;
                   j=l;//to prevent multiple printing 
                   number_of_location_found++;
               }
              /*
              if(nodes[i].name.substr(j,n) == location_name)
              {
                  cout<<"\nName: "<<nodes[i].name<<"\tID: "<<nodes[i].id<<endl;
                  j=l;
              }
              */
          }
      }
  }
  if(number_of_location_found == 0)cout<<"\n\nSORRY COULDN'T FIND ANY LOCATION"<<endl;

}
///finding distance between node provided by user and other nodes
void distances(int index)
{    
    list_of_distace.clear();//erase all the element in vector list_of_distance.
    ////cout<<"\n"<<list_of_distace.size();
    for(int i=0; i<nodes.size();i++)
    {
        if(i == index) continue;

        long double node_distance;
        node_distance = sqrt( pow((nodes[i].lat - nodes[index].lat),2) + pow((nodes[i].lon - nodes[index].lon),2));

        pair<long double, NodeData> new_node_distace;
        new_node_distace = make_pair(node_distance , nodes[i]);

        list_of_distace.push_back(new_node_distace);

    }

}
////comparing two values
 bool compare(pair<long double, NodeData> node1, pair<long double, NodeData> node2)
 {
     return(node1.first <node2.first);
 }