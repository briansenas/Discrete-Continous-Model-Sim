#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
// #include <list>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <limits.h>

using namespace std;

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

// calcula por montecarlo el beneficio esperado de un juego en el que el que lanza monedas paga x euros por lanzamiento, el juego continua hasta que la diferencia entre el número de caras y de cruces (en valor absoluto) es igual a dif, y el otro jugador le paga al primero y euros al finalizar

float prob; //probabilidad de obtener cara
int dif; //diferencia entre el numero de caras y de cruces para parar los lanzamientos
int lanzamientos; //lanzamientos de moneda realizados
float esperado; //beneficio medio obtenido
float medialanzamientos; //numero medio de lanzamientos
int i;
int veces; //numero de repeticiones del juego
int caras, cruces; //numero de caras y cruces obtenidas
float pagoporlanzamiento,pagoalfinalizar;


int width = 20, precision = 6;

int lanza(){
    float u;
    u = (float) random();
    u = u/(float)(RAND_MAX+1.0);
    if (u<prob)
        return 0;
    else return 1;
}


/* Programa principal */
int main(int argc, char *argv[])
{
    veces=100000;
    prob=0.5;
    dif=3;
    pagoporlanzamiento=10.0;
    pagoalfinalizar=100.0;
    char apartado = 'a';
    int streambus;
    if (argc>=2){
        apartado = *argv[1];
        if(argc>=3)
            streambus = atoi(argv[2]);
    }
    else if(argc >= 6)
    {
        sscanf(argv[3],"%d",&veces);
        sscanf(argv[4],"%f",&pagoporlanzamiento);
        sscanf(argv[5],"%f",&pagoalfinalizar);
        prob=0.5;
        dif=3;
    }
    else if(argc!=1 and argc != 8)
    {
        printf("\nFormato Argumentos -> <numero de iteraciones> <pago por lanzamiento> <pago al finalizar> <probabilidad de cara> <diferencia entre caras y cruces\n");
        exit(1);
    }
    else
    {
        sscanf(argv[3],"%d",&veces);
        sscanf(argv[4],"%f",&pagoporlanzamiento);
        sscanf(argv[5],"%f",&pagoalfinalizar);
        sscanf(argv[6],"%f",&prob);
        sscanf(argv[7],"%d",&dif);
    }


    // Deberiamos escribir la semilla que vayamos utilizar
    //srandom(123456);
    srandom(time(NULL));
    esperado=0.0;
    medialanzamientos=0;

    ostringstream oss;
    float obtenido;
    int repetir=1;

    ofstream myfile;
    string writefile = "";
    ostringstream name;
    string path;
    if(streambus==1){
        path = get_selfpath();
        path = path.substr(0,path.find_last_of("/\\") + 1);
    }
    int* val_y, *val_z;
    int prob_stp = 8, diff_stp = 10, step = 10, pfrepeat=0,plrepeat=0;
    float probs[prob_stp]; int diffs[diff_stp];
    switch(apartado){
        case 'a':{
                     cout << "Número de veces a repetir el bucle: ";
                     cin >> repetir;
                     cout << "Número de veces a jugar: ";
                     cin >> veces;
                     cout << endl;

                     name << char(toupper(apartado))<<"-" << "Coin-" << veces;
                     string datafilename = name.str();
                     writefile = path+"./coin_results/"+datafilename+".txt";
                     cout << "[WARNING]: Writting to file: " << writefile << endl;
                     myfile.open(writefile,ios::out|ios::trunc);
                     // Rows to skip ahead and columns
                     myfile << std::left << setw(width) << 3 << setw(width) << 2 << endl;
                     myfile << std::left << setw(width) << "PerThrow" << setw(width) << "PerFinish" << setw(width) << "Odds" << setw(width) << "Diff" << endl;
                     myfile << std::left << setw(width) << setprecision(precision) << pagoporlanzamiento <<
                         setw(width) << pagoalfinalizar << setw(width) <<
                         prob << setw(width) << dif  << endl;
                     myfile << std::left << setw(width) << "Expected" << setw(width) << "Avg.Throw" << endl;
                     break;
                 }
        case 'b':{
                     pfrepeat = 20, plrepeat=10, step=10;
                     pagoalfinalizar = 200;
                     val_y = new int[plrepeat];
                     val_z = new int[pfrepeat];
                     for(unsigned int i=0;i<plrepeat;i++){
                         val_y[i] = pagoporlanzamiento + (i)*step;
                     }
                     for(unsigned int i=0;i<pfrepeat;i++){
                         val_z[i] = pagoalfinalizar - (i)*step;
                     }
                     name << char(toupper(apartado))<<"-" << "Coin";
                     string datafilename = name.str();
                     writefile = path+"./coin_results/"+datafilename+".txt";
                     cout << "[WARNING]: Writting to file: " << writefile << endl;
                     myfile.open(writefile,ios::out|ios::trunc);
                     // Rows to skip ahead and columns
                     myfile << std::left << setw(width) <<  3 << setw(width) << 3 << endl;
                     myfile << std::left << setw(width) << "Odds" << setw(width) << "Diff"  << setw(width) << "Steps" << endl;
                     myfile << std::left << setw(width) << setprecision(precision) << prob << setw(width) << dif << setw(width) << plrepeat << endl;
                     myfile << std::left << setw(width) << "PerThrow" << setw(width) << "PerFinish" << setw(width) << "Expected" << endl;
                     break;
                 }
        case 'c':{
                     dif = 2;
                     prob = 0.1;
                     repetir = 1;
                     probs[0] = 0.2;
                     diffs[0] = 3;
                     for(unsigned int i=1;i<prob_stp;i++){
                         probs[i] = probs[i-1]+0.1;
                     }
                     for(unsigned int i=1;i<diff_stp;i++){
                         diffs[i] = diffs[i-1]+1;
                     }
                     name << char(toupper(apartado))<<"-" << "Coin";
                     string datafilename = name.str();
                     writefile = path+"./coin_results/"+datafilename+".txt";
                     cout << "[WARNING]: Writting to file: " << writefile << endl;
                     myfile.open(writefile,ios::out|ios::trunc);
                     // Rows to skip ahead and columns
                     myfile << std::left << setw(width) << "3" << setw(width) << "4" << endl;
                     myfile << std::left << setw(width) << "PerThrow" << setw(width)
                         << "PerFinish" << setw(width) << "Step" << endl;
                     myfile << std::left << setw(width) << setprecision(precision) << pagoporlanzamiento <<
                         setw(width) << pagoalfinalizar << setw(width) << prob_stp << endl;
                     myfile << std::left <<setw(width) << "Iter" << setw(width)  << setw(width) << "Odds" << setw(width) << "Diff" <<
                         setw(width) << "Expected" << setw(width)<< "Avg.Throw" << endl;
                    break;
                 }
        default:{
                    cout << "[WARNING]: Apartado no identificado" << endl;
                    break;
                }
    }

    int pstp = 0,dstp = 0, pl = 1, pf = 0, r=0;
    do{
        do{
            do{
                do{
                    for (unsigned int vez=0; vez<veces; vez++) {
                        lanzamientos = 0;
                        caras = 0;
                        cruces = 0;
                        while (abs(caras-cruces) != dif) {
                            i=lanza();
                            lanzamientos++; // Número de lanzamientos
                            if (i==0) caras++;
                            else cruces++;
                        }
                        // Escribir lso valores esperados por números de lanzamientos?
                        obtenido = pagoalfinalizar-lanzamientos*pagoporlanzamiento;
                        esperado +=obtenido;
                        medialanzamientos += lanzamientos;

                        if(streambus==1 and apartado=='?'){
                            myfile << std::left << setw(width) << vez << setw(width) << obtenido << setw(width) << lanzamientos  << setw(width) << esperado << endl;
                        }
                    }

                    //Escribinir los valores promedios obtenidos ???
                    esperado=esperado/veces;
                    medialanzamientos = medialanzamientos/veces;

                    if(apartado=='b'){
                        myfile << std::left << setw(width) << setprecision(precision) << pagoporlanzamiento <<
                            setw(width) << pagoalfinalizar << setw(width) << esperado << endl;

                        // Generate gnuplot block
                        if(apartado=='b' and pl == 0)
                            myfile << "\n\n";

                        pagoporlanzamiento = val_y[pl++];
                        pagoalfinalizar = val_z[pf];

                    }

                    if(apartado=='a')
                        myfile << std::left << setw(width) << r << setw(width) << setprecision(precision) << esperado << setw(width) << medialanzamientos << endl;

                    if(!apartado=='c'){
                        esperado = 0;
                        medialanzamientos = 0;
                    }
                    r++;
                }while( (pl<plrepeat && apartado=='b') or (apartado=='a' and r < repetir));
                pl = 0;
                pf++;
            }while(pf<pfrepeat && apartado=='b');

            if(apartado=='c'){

                myfile << std::left << setw(width) << dstp << setw(width) << setprecision(precision) << prob << setw(width) <<
                    dif << setw(width) << esperado << setw(width) << medialanzamientos << endl;

                prob = probs[pstp];
                if(dstp==diff_stp)
                    dstp = 0;
                dif = diffs[dstp];

                esperado = 0;
                medialanzamientos = 0;
            }
            dstp++;
        }while(dstp<diff_stp && apartado=='c');
        pstp++;
        // if(apartado=='c' and pstp!=prob_stp)
            // myfile << "\n\n";
    }while(pstp<prob_stp && apartado=='c');

    myfile.close();
}
