#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <limits.h>

using namespace std;

//https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
void progress_bar(float progress){
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

//https://stackoverflow.com/questions/5525668/how-to-implement-readlink-to-find-the-path
string get_selfpath() {
    char buff[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len != -1) {
        buff[len] = '\0';
        return std::string(buff);
    }
    /* handle error condition */
    return "";
}


//modelo del lago con peces chicos y grandes, cambiando los par�metros
// Definition of additional mathematicals functions
#define sq(X) {int aux=X; (X)*(X); }
#define dec(X) pow(10, X )
#define ctg(X) (1.0/tan(X))
#define actg(X) atan(1.0/(X))
#define sqn(X,Y) pow( X, 1.0/(Y) )
#define lgn(X,Y)  (log(Y)/log(X))


// Variables for time control
double _t = 0.0;
double _tinicio = 0.0;
double _tfin = 0.0;
double _inc_t = 0.0;
int _tcom = 1;

// Variables of Parameters
double __A = 0.0666667;
double __B = 5.0E6;
double __C = 0.0222222;
double __D = 5.0E6; //
double __E = 36000.0;
double __F = 5.0;
double pescar = 0.0;
int frecuencia = 0.0;
double max_pragm = 1100;
double max_frec = 10;
double max_pescado = 16000;
double Estable = 3000;

double fishes  = 0.0;
double sharks = 0.0;

// Vectors of model's variables
double *  __x = new double[1];
double *  __y = new double[1];

double *  x_aux = new double[1];
double *  y_aux = new double[1];



// Functions for variables higher derivative calculation

// Function for calculation of x's higher derivative
double f1 ( double *  __x, double *  __y, double _t ){
    return ( ( ( __A ) * ( __x[0] ) ) * ( ( 1.0 ) - ( ( __x[0] ) / ( __B ) ) ) ) - ( ( __F ) * ( __y[0] ) ) ;
}

// Function for calculation of y's higher derivative
double f2 ( double *  __x, double *  __y, double _t ){
    return ( ( __C ) * ( __y[0] ) ) * ( ( ( __x[0] ) / ( __D ) ) - ( ( __y[0] ) / ( __E ) ) ) ;
}

// Function with Runge-Kutta of 4� order algorithm implementation for model.
void  resolver (double _t0, double _tf, double * _res ){
    //		double *  x_aux = new double[1];
    //		double *  y_aux = new double[1];
    double k1[2][1];
    double k2[2][1];
    double k3[2][1];
    double k4[2][1];

    double h = _inc_t ;


    if(_t0>Estable&&int(_t0)%int(frecuencia)==0)
        __y[0] -= pescar;

    for(_t=_t0; _t<(_tf-10e-8); _t+=_inc_t){
        //		for(_t=_t0; _t<_tf; _t+=_inc_t){
        //cout.precision(20);
        //cout << " tactual " << _t << " tfin es " << _tf << endl;
        //cout.precision(3);
        x_aux[0] = __x[0];
        y_aux[0] = __y[0];
        k1[0][0] = h * f1( __x, __y,_t );
        k1[1][0] = h * f2( __x, __y,_t );

        // PREVIO CODIGO DE K2
        __x[0] = (x_aux[0]+k1[0][0] / 2);
        __y[0] = (y_aux[0]+k1[1][0] / 2);

        // CODIGO DE K2
        k2[0][0] = h * f1( __x, __y,_t + (h/2) );
        k2[1][0] = h * f2( __x, __y,_t + (h/2) );

        // PREVIO CODIGO DE K3
        __x[0] = (x_aux[0]+k2[0][0] / 2);
        __y[0] = (y_aux[0]+k2[1][0] / 2);

        // CODIGO DE K3
        k3[0][0] = h * f1( __x, __y,_t + (h/2) );
        k3[1][0] = h * f2( __x, __y,_t + (h/2) );

        // PREVIO CODIGO DE K4
        __x[0] = (x_aux[0]+k3[0][0]);
        __y[0] = (y_aux[0]+k3[1][0]);

        // CODIGO DE K4
        k4[0][0] = h * f1( __x, __y,_t + h);
        k4[1][0] = h * f2( __x, __y,_t + h);

        // CODIGO FINAL
        __x[0] = x_aux[0];
        __x[0] += (k1[0][0]+2*k2[0][0]+2*k3[0][0]+k4[0][0])/6;
        __y[0] = y_aux[0];
        __y[0] += (k1[1][0]+2*k2[1][0]+2*k3[1][0]+k4[1][0])/6;
        if (__x[0]<0.0) __x[0] = 0.0;
        if (__y[0]<0.0) __y[0] = 0.0;
    }
    //cout << " termino el bucle de t= " << _tf << endl;
    //		delete  x_aux;
    //		delete  y_aux;
    _res[0] = _t;
    _res[1] = __x[0];
    _res[2] = __y[0];
    }

    // Main: execution (with text interface) of the model's simulation.
    int main(int argc, char** argv){

        //	cerr << " tinicio => ";
        //	cin >> _tinicio;
        //	cout << " tinicio " << _tinicio << endl;
        _tinicio = 0.0;

        //	cerr << " tfin => ";
        //	cin >> _tfin;
        //	cout << " tfin " << _tfin << endl;
        //cerr << " Duracion de la simulacion => ";
        //cin >> _tfin;

        //	cerr << " Incremento de tiempo, inc_t => ";
        //	cin >> _inc_t;
        //	if(_inc_t <= 0.0){
        //		cerr << "Error: _int_t <= 0.0" << endl ;
        //		exit(0);
        //	}
        //	cout << " inc_t " << _inc_t << endl;
        _inc_t = 0.1;

        //	cerr << " Intervalo de comunicacion, tcom => ";
        //	cin >> _tcom;
        //	if(_tcom <= 0){
        //		cerr << "Error: _tcom < 1" << endl ;
        //		exit(0);
        //	}
        //	cout << " tcom " << _tcom << endl;
        _tcom = 10;

        // cerr << " Numero inicial de peces peque�os, x(0) => ";
        // cin >> __x[0];
        //	cout << " x(0) "<< __x[0] << endl;


        // cerr << " Numero inicial de peces grandes, y(0) => ";
        // cin >> __y[0];

        if(argc<=3){
            cerr << "[ERROR]: Expected 3 parameters, [Days] [fish] [sharks]" << endl;;
            exit(-1);
        }
        _tfin = atof(argv[1]);
        __x[0] = atof(argv[2]);
        __y[0] = atof(argv[3]);
        fishes = __x[0];
        sharks = __y[0];



        //	cout << " y(0) "<< __y[0] << endl;
        string path = get_selfpath();
        path = path.substr(0,path.find_last_of("/\\") + 1);

        stringstream oss;
        oss << std::fixed << setprecision(0) << path << "river_results/optimo.txt";
        ofstream data;  data.open(oss.str(),ios::out|ios::trunc);
        data << std::fixed << std::left << setprecision(0) << setw(30) << 0 << setw(30) <<  __x[0]  << setw(30) << __y[0] << endl;

        /*
           cerr << " A_inic => ";
           cin >> __A;
           cout << " A " << __A << endl;

           cerr << " B_inic => ";
           cin >> __B;
           cout << " B " << __B << endl;

           cerr << " C_inic => ";
           cin >> __C;
           cout << " C " << __C << endl;

           cerr << " D_inic => ";
           cin >> __D;
           cout << " D " << __D << endl;

           cerr << " E_inic => ";
           cin >> __E;
           cout << " E " << __E << endl;

           cerr << " F_inic => ";
           cin >> __F;
           cout << " F " << __F << endl;
            double max_pragm = 1100;
            double max_frec = 10;
            double max_pescado = 16500;
            double Estable = 2000;

            double fishes  = 0.0;
            double sharks = 0.0;
           */
        int max_days = max_pescado/max_pragm * max_frec;
        double inc = _inc_t*_tcom;
        double * aux = new double[2];
        double factor = (max_pragm/max_frec);
        bool maximo = false;
        double maximos[max_days];
        for(int i=0;i<max_days;i++){
            maximos[i] = 0;
        }
        for(int j=1;j<1000;j++){
            maximo = false;
            for(int i=1;i<max_days;i++){
                __x[0] = fishes;
                __y[0] = sharks;
                frecuencia = i;
                pescar = i*factor+j;
                for(double t=_tinicio ; t < (_tfin-10e-8) ; t += inc ){
                    resolver( t, t + inc, aux );
                }
                if(aux[2]>0){
                    maximo = true;
                    maximos[i] = pescar;
                    break;
                }
                progress_bar(float(j)/float(1000));
            }
            if(!maximo){
                progress_bar(1);
                break;
            }

        }
        cout << endl;

        double best_rate = -1;
        double _real_max[2];
        double rate = 0;
        for(int i=1;i<max_days;i++){
            rate = maximos[i]/i;
            // cout << "Frecuencia: " << i << " con: " << rate << endl;
            if(rate>best_rate){
                best_rate = rate;
                _real_max[0] = i;
                _real_max[1] = maximos[i];
            }

        }

        cout << "Best_rate: " << best_rate << " - " << _real_max[0] << " -> " << _real_max[1] << endl;

    }
