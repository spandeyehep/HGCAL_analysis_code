#define PI 3.14159265
#define TEST_BEAM_LAYER_ROTATION -PI/2

std::vector<int> types_cell;
std::vector<std::pair<double,double> > xypos_cell;
std::vector<std::pair<double,double> > xypos_wafer;


class HGCalTestNumbering {
public:
  static const int kHGCalCellSOffset = 0;
  static const int kHGCalCellSMask = 0xFFFF;
  static const int kHGCalSectorSOffset = 16;
  static const int kHGCalSectorSMask = 0x7F;
  static const int kHGCalSubSectorSOffset = 23;
  static const int kHGCalSubSectorSMask = 0x1;
  static const int kHGCalLayerSOffset = 24;
  static const int kHGCalLayerSMask = 0x7F;
  static const int kHGCalZsideSOffset = 31;
  static const int kHGCalZsideSMask = 0x1;

  static const int kHGCalCellHOffset = 0;
  static const int kHGCalCellHMask = 0xFF;
  static const int kHGCalCellTypHOffset = 8;
  static const int kHGCalCellTypHMask = 0x1;
  static const int kHGCalWaferHOffset = 9;
  static const int kHGCalWaferHMask = 0x3FF;
  static const int kHGCalLayerHOffset = 19;
  static const int kHGCalLayerHMask = 0x7F;
  static const int kHGCalZsideHOffset = 26;
  static const int kHGCalZsideHMask = 0x1;
  static const int kHGCalSubdetHOffset = 27;
  static const int kHGCalSubdetHMask = 0x7;
  HGCalTestNumbering() {}
  virtual ~HGCalTestNumbering() {}
  // static uint32_t packSquareIndex(int z, int lay, int sec, int subsec, int cell);
  // static uint32_t packHexagonIndex(int subdet, int z, int lay, int wafer, int celltyp, int cell);
  static void unpackSquareIndex(const uint32_t& idx, int& z, int& lay, int& sec, int& subsec, int& cell);
  static void unpackHexagonIndex(
      const uint32_t& idx, int& subdet, int& z, int& lay, int& wafer, int& celltyp, int& cell);
  // static bool isValidSquare(int z, int lay, int sec, int subsec, int cell);
  // static bool isValidHexagon(int subdet, int z, int lay, int wafer, int celltyp, int cell);
};



void HGCalTestNumbering::unpackHexagonIndex(
    const uint32_t& idx, int& subdet, int& zp, int& lay, int& wafer, int& celltyp, int& cell) {
  cell = (idx >> kHGCalCellHOffset) & kHGCalCellHMask;
  celltyp = (idx >> kHGCalCellTypHOffset) & kHGCalCellTypHMask;
  wafer = (idx >> kHGCalWaferHOffset) & kHGCalWaferHMask;
  lay = (idx >> kHGCalLayerHOffset) & kHGCalLayerHMask;
  zp = ((idx >> kHGCalZsideHOffset) & kHGCalZsideHMask ? 1 : -1);
  subdet = (idx >> kHGCalSubdetHOffset) & kHGCalSubdetHMask;
}


void HGCalTestNumbering::unpackSquareIndex(const uint32_t& idx, int& zp, int& lay, int& sec, int& subsec, int& cell) {
  cell = (idx >> kHGCalCellSOffset) & kHGCalCellSMask;
  subsec = ((idx >> kHGCalSubSectorSOffset) & kHGCalSubSectorSMask ? 1 : -1);
  sec = (idx >> kHGCalSectorSOffset) & kHGCalSectorSMask;
  lay = (idx >> kHGCalLayerSOffset) & kHGCalLayerSMask;
  zp = ((idx >> kHGCalZsideSOffset) & kHGCalZsideSMask ? 1 : -1);
}

class HGCalTBCellVertices
{
public:

        HGCalTBCellVertices();  ///< Constructor from cell \b iu & \b iv, valid sensorSizes are 128 and 256                                   

        std::vector<std::pair<double, double>> GetCellCoordinates(int layer, int sensor_iu, int sensor_iv, int iu, int iv, int sensorsize, bo\
ol flipX = false); ///< returns the coordinates of each vertex of cell in the lab frame \b (x,y)                                              

        inline std::vector<std::pair<double, double>> GetCellCoordinatesForPlots(int layer, int sensor_iu, int sensor_iv, int iu, int iv, int\
 sensorsize)
        {
                return GetCellCoordinates(layer, sensor_iu, sensor_iv, iu, iv, sensorsize, true);
        };  ///< returns the coordinates of each vertex of cell in the lab frame \b (x,y)                                                     

        std::pair<double, double> GetCellCentreCoordinates(int layer, int sensor_iu, int sensor_iv, int iu, int iv, int sensorsize, bool flip\
X = false); ///< returns the center of the cell in absolute coordinates: \b (x,y)                                                             
        std::pair<int, int> GetCellIUIVCoordinates(double x, double y);
        std::pair<int, int> GetSensorIUIVCoordinates(double X, double Y);

        inline  std::pair<double, double> GetCellCentreCoordinatesForPlots(int layer, int sensor_iu, int sensor_iv, int iu, int iv, int senso\
rsize)
        {
                return GetCellCentreCoordinates(layer, sensor_iu, sensor_iv, iu, iv, sensorsize, true);
        }; ///< returns the center of the cell in absolute coordinates: \b (x,y)                                                              


//  void CellType(int iu, int v, bool ValidFlag);// 1 for full hex, 2 for half hex and 3 for the pentagons(to be implemented later)           
private:
  //double a = HGCAL_TB_CELL::FULL_CELL_SIDE; // Size in terms of 1 unit of x/y co-ordinates of a cell side which is 0.064 cm
  double a = 0.064;
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

        double Xmax(int iv, double y);// returns the max x value for a cell to be in the given sensor                                         

        std::pair<double, double> RotateLayer(std::pair<double, double>, double Angle);

};

HGCalTBCellVertices::HGCalTBCellVertices()
{
// Initialize the co-ordinates of a hexagonal cell of side a centred at 0,0 with the top vertex numbered as the first with clockwise incremen\
ts.                                                                                                                                           
        double x1[] = {0., x_a * a, x_a * a, 0., -x_a * a, -x_a * a};
        double y1[] = {a,  y_a * a, -y_a * a, -a, -y_a * a, y_a * a};
        for(int iii = 0; iii < 6; iii++) { // May have to be generalized to deal with polygons of any size                                    
                x_co_FullHex.push_back(x1[iii]);
                y_co_FullHex.push_back(y1[iii]);
        }

}



std::pair<double, double> HGCalTBCellVertices::RotateLayer(std::pair<double, double> Vertex, double Angle)
{
        double X_new = (Vertex.first) * cos(Angle) - (Vertex.second) * sin(Angle);
        double Y_new = (Vertex.first) * sin(Angle) + (Vertex.second) * cos(Angle);
        return std::make_pair(-X_new, Y_new);// The negative sign for the x coordinate is to account for the TB cartesian coordinate system.  
}



std::pair<int, int> HGCalTBCellVertices::GetCellIUIVCoordinates(double _x, double _y) {
  double rotation = TEST_BEAM_LAYER_ROTATION;
  auto point = RotateLayer(std::make_pair(-_x, _y), -rotation);
  double x = -point.first;
  double y = point.second;
  double iv = y * 1/vy0;
  double iu = (x-vx0*iv)/x0;

  return std::make_pair(round(iu), round(iv));

}

std::pair<int, int> HGCalTBCellVertices::GetSensorIUIVCoordinates(double _X, double _Y) {  
  double rotation = TEST_BEAM_LAYER_ROTATION;
  auto point = RotateLayer(std::make_pair(-_X, _Y), -rotation);
  double X = -point.first;
  double Y = point.second;
  double iV = Y * 1/VY0;
  double iU = (X-VX0*iV)/X0;


  return std::make_pair(round(iU), round(iV));
  
}


//spandey 
void initWaferGeom(){
  const int nC(3);
  int nCoarse(3), nyCoarse(3);
  int cellCoarse[nC] = {2,3,2};
  //FULL_CELL_SIDE = 0.6496345
  //double wafer(3*19*HGCAL_TB_CELL::FULL_CELL_SIDE);
  double wafer(3*19*0.6496345);

  int    rows = nC;
  double cell = wafer/nCoarse;
  double dx   = 0.5*cell;
  double dy   = 0.5*cell*tan(30.0*M_PI/180.0);
  int    ny   = nyCoarse;


  xypos_wafer.push_back(std::pair<double,double>(0,0));

  
  for (int ir = 0; ir < rows; ++ir) {
    int    column = cellCoarse[ir];
    int    nx     = 1 - column;
    double ypos   = dy*ny;
    for (int ic = 0; ic<column; ++ic) {
      double xpos = dx*nx;
      nx += 2;
      
      if(ir==1 && ic==1) 
  continue;

      //xypos.push_back(std::pair<double,double>(xpos,ypos));
      xypos_wafer.push_back(std::pair<double,double>(ypos,xpos));  ///currently we have rotated the geometry by 90 degrees so x becomes y and y becomes x.
      
    }
    //ny += 6;
    ny -= 3;
  }
  // std::cout << "Initialize HexGeometry for a wafer " << xypos_wafer.size() << " wafer"
  //     << std::endl;

  // for (int ir = 0; ir < 7; ++ir) {
  //   std::cout<<"ir : X : Y : "<<ir<<" "<<xypos_wafer[ir].first<<" "<<xypos_wafer[ir].second<<std::endl;
  // }



}





void checkCell() {


  
  const int nC(15), nF(20);
  int rows = 15;
double wafer = 12.3424;

double cell = 1.122;
double dx = 0.56;
double dy = 0.005127;
int ny = 21;
 bool fine = false;
  int cellCoarse[nC] = {2,5,8,11,12,11,12,11,12,11,12,11,8,5,2};
  int cellFine[nF] = {3,6,9,12,15,16,15,16,15,16,15,16,15,16,15,14,11,8,5,2};

    for (int ir = 0; ir < rows; ++ir) {
    int    column = (fine) ? cellFine[ir] : cellCoarse[ir];
    int    nx     = 1 - column;
    double ypos   = dy*ny;
    for (int ic = 0; ic<column; ++ic) {
      double xpos = dx*nx;
      nx += 2;
      //xypos.push_back(std::pair<double,double>(xpos,ypos));                                                                                 
      xypos_cell.push_back(std::pair<double,double>(ypos,xpos));  ///currently we have rotated the geometry by 90 degrees so x becomes y and \
y becomes x.                                                                                                                                  

      int type = 0;
      if (ir==0) type = 2;
      else if (ir==1 && (ic==0 || ic==4)) type = 2;
      else if (ir==2 && (ic==0 || ic==7)) type = 2;
      else if (ir==3 && (ic==0 || ic==10)) type = 2;
      else if (ir==4 && (ic==0 || ic==11)) type = 2;
      else if (ir==6 && (ic==0 || ic==11)) type = 2;
      else if (ir==8 && (ic==0 || ic==11)) type = 2;
      else if (ir==10 && (ic==0 || ic==11)) type = 2;
      else if (ir==11 && (ic==0 || ic==10)) type = 2;
      else if (ir==12 && (ic==0 || ic==7)) type = 2;
      else if (ir==13 && (ic==0 || ic==4)) type = 2;
      else if (ir==14) type = 2;

      types_cell.push_back(type);

    }
    //ny += 6;                                                                                                                                
    ny -= 3;
  }
  // std::cout << "Initialize HexGeometry for a cell " << xypos_cell.size() << " cells"
  //     << std::endl;


  initWaferGeom();
}






std::pair<double,double> position_cell(const int cell) {
  checkCell();
  std::pair<double,double> xy;
  if (cell >= 0 && cell < (int)(xypos_cell.size())) {
    xy = xypos_cell[cell];
  } else {
    xy = std::pair<double,double>(0,0);
  }

  
  return xy;
}


std::pair<double,double> position_wafer(const int wafer) {
  std::pair<double,double> xy;
  if (wafer >= 0 && wafer < (int)(xypos_wafer.size())) {
    xy = xypos_wafer[wafer];
  } else {
    xy = std::pair<double,double>(0,0);
  }
  return xy;
}



void getCellID(uint32_t idx){
  int subdet, z, layer, wafer, celltype, cellno;
  HGCalTestNumbering::unpackHexagonIndex(idx, subdet, z, layer, wafer, celltype, cellno);


  // cout<<xy.first<<"\t"<<xy.second<<endl;
    HGCalTBCellVertices Thecell;
    std::pair<double,double> xy = position_cell(cellno);
  std::pair<int, int> iuiv = Thecell.GetCellIUIVCoordinates(xy.first,xy.second);;
  // cout<<iuiv.first<<"\t"<<iuiv.second<<endl;

  std::cout << idx << "  layer: " << layer << "   wafer:  " << wafer << "   cellno:  " << cellno << "  (x,y): (" << xy.first << "," <<  xy.second<<")    (iu,iv):  ("<<iuiv.first<< "," <<iuiv.second<<")"<< std::endl;
  

}
