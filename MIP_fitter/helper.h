std::map<int, std::vector<int>> module_map;

void moduleMap_init(const char* config) {
  char *f_name = new char[200];

  if(strcmp(config,"alpha")==0 || strcmp(config,"config1")==0) {
    sprintf(f_name,"../config_maps/moduleMAP_config1.txt");
    cout<<"\n\nINFO: Mapping module configuration ALPHA (oct10-oct17) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else if(strcmp(config,"bravo")==0 || strcmp(config,"config2")==0) {
    sprintf(f_name,"../config_maps/moduleMAP_config2.txt");
    cout<<"\n\nINFO: Mapping module configuration BRAVO (17oct-22oct) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else if(strcmp(config,"charlie")==0  || strcmp(config,"config3")==0) {
    sprintf(f_name,"../config_maps/moduleMAP_config3.txt");
    cout<<"\n\nINFO: Mapping module configuration CHARLIE (23Oct-4Nov) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else {
    cout<<"\n\nERROR: Incorrect configuration entered "<<endl;
    cout<<" Allowed configuration :\n alpha = Configuration 1 (10Oct-17Nov) \n bravo = Configuration 2 (17Oct-22Oct) \n charlie = Configuration 3 (23Oct-17Nov)"<<endl;
    return;
    
  }

  std::ifstream in(f_name);
  if(!in){
    cout<<"Could not find "<<f_name<<endl;
    return;
  }
  int modID_, part_, layer_, pos_;
  cout<<"File name = "<<f_name<<endl;
  while(in>>modID_>>part_>>layer_>>pos_){
    std::pair<int, std::vector<int>> temp_pair;
    std::vector<int> temp_vector;
    temp_vector.push_back(part_);
    temp_vector.push_back(layer_);
    temp_vector.push_back(pos_);
    temp_pair = std::make_pair(modID_,temp_vector);
    module_map.insert(temp_pair);
  }

  cout<<"INFO: Module Mapping Done!!! "<<endl<<endl;


}


std::vector<int> getModuleLocation(int moduleID) {
  std::map<int, std::vector<int>>::iterator it = module_map.find(moduleID);
  if(it != module_map.end()){
    return it->second;
  }
  else {
    return std::vector<int>();
  }
}
