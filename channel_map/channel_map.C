//#include "mapping.h"
#include "math.h"

#define PI 3.14159265
#define TEST_BEAM_LAYER_ROTATION -PI/2

using namespace std;

double delta = 0.00001;//needed for comparing two doubles while deciding if the cell is within a sensor
double a = 0.6496345;  //in cm
double A = 11*a; // One side of a full sensor(neglecting the cut at the MB)
double x_a = sqrt(3) / 2; // cosine pi/6
double y_a = 1 / 2.; // sine pi/6
double vy_a = 3. / 2;

std::vector<double> x_co_FullHex, y_co_FullHex; // stores the initial x,y coordinates of a hexagonal cell
std::vector<std::pair<double, double>> Cell_co;
// Translation in u,v co-ordinates in terms of TB cartesian -x,y.
double  x0 = 2 * x_a * a; //Translation in Cartesian x for 1 unit of iu
double vx0 = x_a * a; // Cartesian x component of translation for 1 unit of iv
double vy0 = vy_a * a; // Cartesian y component of translation for 1 unit of iv
// Translation in Sensor_u, Sensor_v co-ordinates in terms of TB cartesian -x,y.
double  X0 = 2 * x_a * A; //Translation in Cartesian x for 1 unit of Sensor_iu
double VX0 = x_a * A; // Cartesian x component of translation for 1 unit of Sensor_iv
double VY0 = vy_a * A; // Cartesian y component of translation for 1 unit of Sensor_iv



void Init()
{
// Initialize the co-ordinates of a hexagonal cell of side a centred at 0,0 with the top vertex numbered as the first with clockwise increments.
	double x1[] = {0., x_a * a, x_a * a, 0., -x_a * a, -x_a * a};
	double y1[] = {a,  y_a * a, -y_a * a, -a, -y_a * a, y_a * a};
	for(int iii = 0; iii < 6; iii++) { // May have to be generalized to deal with polygons of any size
		x_co_FullHex.push_back(x1[iii]);
		y_co_FullHex.push_back(y1[iii]);
	}

}


bool iu_iv_valid(int layer, int sensor_iu, int sensor_iv, int iu, int iv, int sensorSize)
{
	int aiv = abs(iv);
	int iuc = (iv < 0) ? (-iu) : (iu);
	if(layer <= 28 && sensor_iu == 0 && sensor_iv == 0) {
		if(sensorSize == 128) {
			if (iv == 0) return (iu >= -5 && iu <= 5);
			else if (aiv == 1) return (iuc >= -6 && iuc <= 5);
			else if (aiv == 2) return (iuc >= -6 && iuc <= 4);
			else if (aiv == 3) return (iuc >= -7 && iuc <= 4);
			else if (aiv == 4) return (iuc >= -7 && iuc <= 3);
			else if (aiv == 5) return (iuc >= -6 && iuc <= 1);
			else if (aiv == 6) return (iuc >= -5 && iuc <= -1);
			else if (aiv == 7) return (iuc == -3 || iuc == -4);
			else return false;
		} else return false;
	} else return false;
}




std::pair<double, double> RotateLayer(std::pair<double, double> Vertex, double Angle)
{
	double X_new = (Vertex.first) * cos(Angle) - (Vertex.second) * sin(Angle);
	double Y_new = (Vertex.first) * sin(Angle) - (Vertex.second) * cos(Angle);
	return std::make_pair(-X_new, Y_new);// The negative sign for the x coordinate is to account for the TB cartesian coordinate system.
}



std::pair<double, double> GetCellCentreCoordinates_old(int layer, int sensor_iu, int sensor_iv, int iu, int iv, int sensorsize, bool flipX)
{
	double centre_x_tmp = 0., centre_y_tmp = 0.;
	bool ValidFlag   = iu_iv_valid(layer, sensor_iu, sensor_iv, iu, iv, 128);
	if(ValidFlag) {
		centre_x_tmp = iu * x0 + iv * vx0;
		centre_y_tmp = iv * vy0;
		auto point = RotateLayer(std::make_pair(centre_x_tmp, centre_y_tmp), TEST_BEAM_LAYER_ROTATION);
		//      if(flipX==true) point.first = - point.first;
		return point;
	} else return std::make_pair(-123456, -123456); //iu_iv_Valid() is sufficient to decide if a given iu,iv is a valid sensor index but this is done if some future need may arise.

}


std::pair<double, double> GetCellCentreCoordinates_new(int layer, int sensor_iu, int sensor_iv, int iu, int iv, int sensorsize, bool flipX)
{
	double centre_x_tmp = 0., centre_y_tmp = 0.;
	bool ValidFlag   = iu_iv_valid(layer, sensor_iu, sensor_iv, iu, iv, 128);
	if(ValidFlag) {   
		if(sensor_iu == 0 && sensor_iv == 0){ 
			centre_x_tmp = ( (iu * x0 + iv * vx0) < 0) ? ((iu * x0 + iv * vx0) + delta) : ((iu * x0 + iv * vx0) - delta)  ;
			centre_y_tmp = ( (iv * vy0) < 0) ? ((iv * vy0) + delta) : ((iv * vy0) - delta);
			centre_x_tmp += sensor_iu*X0 + sensor_iv*VX0;
			centre_y_tmp += sensor_iv*VY0;
			auto point = RotateLayer(std::make_pair(centre_x_tmp, centre_y_tmp), TEST_BEAM_LAYER_ROTATION);
			return point;
		}
		else{
			sensor_iu = 0;
			sensor_iv = 0;
			centre_x_tmp = ( (iu * x0 + iv * vx0) < 0) ? ((iu * x0 + iv * vx0) + delta) : ((iu * x0 + iv * vx0) - delta)  ;
												centre_y_tmp = ( (iv * vy0) < 0) ? ((iv * vy0) + delta) : ((iv * vy0) - delta);
												centre_x_tmp += sensor_iu*X0 + sensor_iv*VX0;
												centre_y_tmp += sensor_iv*VY0;
												auto point = RotateLayer(std::make_pair(centre_x_tmp, centre_y_tmp), -PI);
												return point; 
			}
		
	} else return std::make_pair(-123456, -123456); //iu_iv_Valid() is sufficient to decide if a given iu,iv is a valid sensor index but this is done if some future need may arise.

}



void channel_map()
{
	Init();
	 char* f_name = new char[200];
	 sprintf(f_name,"/home/shubham/work/HGCAL/CERN_TB/electronic_map/map_CERN_Hexaboard_June_28Sensors_28EELayers_V0.txt");		//Path to electronic map !!! >> 
	 //sprintf(f_name,"/home/shubham/work/HGCAL/CERN_TB/electronic_map/EMAP_18_aug_2018_with_calibration_pad.txt");		//Path to electronic map !!! >> 
	 //std::ifstream in("/home/shubham/work/HGCAL/CERNTB/channel_map_code/txt_files/calib_file_layer_0.txt");
	 //std::ifstream in("/home/shubham/work/HGCAL/CERNTB/channel_map_code/calib_file_layer_0.txt");
	 std::ifstream in(f_name);

	 if(in) cout<<"File Found!!!"<<endl;
	 
	 int chip, channel, layer, sensor_ix, sensor_iv, iu, iv, type;
	 int count = 0;
	 while(in>>chip>>channel>>layer>>sensor_ix>>sensor_iv>>iu>>iv>>type) {
		 if ((layer-1) != 0) continue;
		 bool isValid = iu_iv_valid(layer, sensor_ix, sensor_iv, iu, iv, 128);
		 std::pair<double,double> cellXY_new = GetCellCentreCoordinates_new(layer,sensor_ix,sensor_iv,iu,iv,128,true);
		 std::pair<double,double> cellXY_old = GetCellCentreCoordinates_old(layer,sensor_ix,sensor_iv,iu,iv,128,true);
		 cout<<"layer_chip_channel : "<<(layer-1)<<"_"<<((chip-1)%4)<<"_"<<channel<<" xx "<<"IU  IV  : " <<iu<<"   "<<iv
		 		 <<" , cellXY_old,cellXY_new = ("<<cellXY_old.first<<","<<cellXY_old.second<<")"<<",("<<cellXY_new.first<<","<<cellXY_new.second<<")"<<endl;
		 count++;
	 }
	 

		cout<<"Total channels = "<<count<<endl;
}
