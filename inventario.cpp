#include "inventario.h"

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

void restart(){
    acum_costototal = 0;
    acum_costopedido = 0;
    acum_costomantenimiento = 0;
    acum_costodeficit = 0;
    acum2_costototal = 0;
    acum2_costopedido = 0;
    acum2_costomantenimiento = 0;
    acum2_costodeficit = 0;

}

//modelo de inventario con politica de pedido estacionaria (s,S)

bool compare(const suc &s1, const suc &s2)
{ return s1.tiempo < s2.tiempo; }

/* Inserta de forma ordenada un elemento en la lista de sucesos */
void insertar_lsuc(suc n)
{
  lsuc.push_back(n);
  // Mantener ordenada la lista por el tiempo de los sucesos
  lsuc.sort(compare);
}

/* Procedimiento temporizacion */
int temporizacion()
{
 int suc_sig;

   suc_sig = lsuc.front().tipo;
   reloj = lsuc.front().tiempo;
   lsuc.pop_front();
   return suc_sig;
}

void suceso(int suc_sig, ofstream& file,int i)
{
      switch(suc_sig)
      {
         case DEMANDA: demanda(); break;
         case EVALUACION: evaluacion(); break;
         case LLEGADA_PEDIDO: llegadapedido(); break;
         case FIN_SIMULACION: generadorInformes(file,i); break;
      }
}


void inicializacion()
{
   reloj = 0.0;
   parar = false;
   nivel = 60;
   pedido = 0;
   tultsuc = 0.0;
   acummas = 0.0;
   acummenos = 0.0;
   acumpedido = 0.0;
   while(!lsuc.empty()) {lsuc.pop_front();}
   nodo.tipo = DEMANDA;
   nodo.tiempo = reloj+generademanda(mediademanda);
   insertar_lsuc(nodo);
   nodo.tipo = EVALUACION;
   nodo.tiempo = reloj+tevaluacion;
   insertar_lsuc(nodo);
   nodo.tipo = FIN_SIMULACION;
   nodo.tiempo = reloj+tparada;
   insertar_lsuc(nodo);
}


void demanda() {
int tam;
 if (nivel > 0)
   acummas += (reloj-tultsuc)*nivel;
 else acummenos += (reloj-tultsuc)*(-nivel);
 tultsuc = reloj;
 tam = generatamano();
 nivel -= tam;
 nodo.tipo = DEMANDA;
 nodo.tiempo = reloj+generademanda(mediademanda);
 insertar_lsuc(nodo);
}

void evaluacion() {
 if (nivel < spequena && pedido == 0) {
    pedido = sgrande - nivel;
    acumpedido += k+i*pedido;
    nodo.tipo = LLEGADA_PEDIDO;
    nodo.tiempo = reloj+generapedido(tpedidomin,tpedidomax);
    insertar_lsuc(nodo);
   }
 nodo.tipo = EVALUACION;
 nodo.tiempo = reloj+tevaluacion;
 insertar_lsuc(nodo);
}

void llegadapedido() {
 if (nivel > 0)
   acummas += (reloj-tultsuc)*nivel;
 else acummenos += (reloj-tultsuc)*(-nivel);
 tultsuc = reloj;
 nivel += pedido;
 pedido = 0;
}

/* Procedimiento generador de informes */
void generadorInformes(ofstream& data, int i)
{
   float costototal, costopedido, costomantenimiento, costodeficit;

   parar = true; //para detener la simulacion
   /* ultimas actualizaciones de contadores estadisticos */
   if (nivel > 0)
     acummas += (reloj-tultsuc)*nivel;
   else acummenos += (reloj-tultsuc)*(-nivel);

   costopedido = acumpedido/tparada;
   costomantenimiento = h*acummas/tparada;
   costodeficit = pi*acummenos/tparada;
   costototal = costopedido+costomantenimiento+costodeficit;

// Imprimir todas las medidas de rendimiento
   // printf("\ncosto total = %.3f",costototal);
   // printf("\ncosto de pedido = %.3f",costopedido);
   // printf("\ncosto de mantenimiento = %.3f",costomantenimiento);
   // printf("\ncosto de deficit = %.3f",costodeficit);
   // printf("\n");

    if(data.is_open())
        data << std::left << setw(10) << i << setw(10) << costototal << setw(10) <<
            costopedido << setw(10) << costomantenimiento << setw(10) << costodeficit << endl ;

   acum_costototal += costototal;
   acum_costopedido += costopedido;
   acum_costomantenimiento += costomantenimiento;
   acum_costodeficit += costodeficit;
   acum2_costototal += costototal*costototal;
   acum2_costopedido += costopedido*costopedido;
   acum2_costomantenimiento += costomantenimiento*costomantenimiento;
   acum2_costodeficit += costodeficit*costodeficit;
}

/* Procedimiento de informe final */
void informe()
/* se encarga de calcular la media y desviacion tipica de los valores obtenidos en las diferentes simulaciones */
{

printf("\n\nValores medios y desviaciones tipicas de las medidas de rendimiento");
printf("\nNumero de simulaciones = %d",veces);

printf("\ncostototal = %.3f,  %.3f",acum_costototal/veces,sqrt((acum2_costototal-acum_costototal*acum_costototal/veces)/(veces-1)));
printf("\ncostopedido = %.3f,  %.3f",acum_costopedido/veces,sqrt((acum2_costopedido-acum_costopedido*acum_costopedido/veces)/(veces-1)));
printf("\ncostomantenimiento = %.3f,  %.3f",acum_costomantenimiento/veces,sqrt((acum2_costomantenimiento-acum_costomantenimiento*acum_costomantenimiento/veces)/(veces-1)));
printf("\ncostodeficit = %.3f,  %.3f",acum_costodeficit/veces,sqrt((acum2_costodeficit-acum_costodeficit*acum_costodeficit/veces)/(veces-1)));
printf("\n");
}


void avg_finforme(ofstream& myfile){
    // Imprimir todas las medidas de rendimiento

    float acum_ct = acum_costototal/veces, acum_ctdsv = sqrt((acum2_costototal-acum_costototal*acum_costototal/veces)/(veces-1));
    float acum_cp = acum_costopedido/veces, acum_cpdsv = sqrt((acum2_costopedido-acum_costopedido*acum_costopedido/veces)/(veces-1));
    float acum_cm = acum_costomantenimiento/veces, acum_cmdsv = sqrt((acum2_costomantenimiento-acum_costomantenimiento*acum_costomantenimiento/veces)/(veces-1));
    float acum_cd =   acum_costodeficit/veces, acum_cddsv = sqrt((acum2_costodeficit-acum_costodeficit*acum_costodeficit/veces)/(veces-1));

    if(myfile.is_open()){
        myfile << std::left << setw(10) << spequena << setw(10) << sgrande <<
            setw(10) << setprecision(6) << acum_ct << setw(10) <<
            acum_ctdsv << setw(10) << acum_cp << setw(10) << acum_cpdsv <<
            setw(10) << acum_cm << setw(10) << acum_cmdsv << setw(10) <<
            acum_cd << setw(10) << acum_cddsv << endl;
    }
}

/* generadores de datos */
// Generador uniforme(01)
float uniforme()
{
  float u;
  u = (float) rand();
  u = u/(float)(RAND_MAX+1.0);
  return u;
}

// Generador de tiempo entre demandas (exponencial)
float generademanda(float media)
{
  float u;
  u = uniforme();
  return(-media*log(1-u));
}

// Generador de tiempo que tarda el pedido (uniforme)
float generapedido(float min, float max)
{
  float u;
  u = uniforme();
  return min+(max-min)*u;
}

// Generador de tamaños de pedido (discreto)
int generatamano()
{
  float u;
  u = uniforme();
  if (u<2.0/6) return 2;
  else if (u<4.0/6) return 3;
       else if (u<5.0/6) return 1;
            else return 4;
}


/* Programa principal */
int main(int argc, char *argv[])
{
 int suc_sig;

 if (argc != 4)
    {
     printf("\n\nFormato Argumentos -> simulaciones spequena sgrande\n\n");
     exit(1);
    }

    veces = atoi(argv[1]);
    spequena = atoi(argv[2]);
    sgrande = atoi(argv[3]);
    srand(time(NULL));

    path = get_selfpath();
    path = path.substr(0,path.find_last_of("/\\") + 1);

    stringstream oss;
    oss << setprecision(2) << path << "inv_results/val1-" << veces << "-" << spequena << "-" << sgrande << ".txt";
    ofstream data;  data.open(oss.str(),ios::out|ios::trunc);
    data << std::left << setw(10) << spequena << setw(10) << sgrande << endl;

    string dir = path + "inv_results/";
    stringstream oss2;
    oss2 << setprecision(2) << dir << "media1-" << veces << "-"  << spequena << "-" << sgrande << ".txt";
    ofstream myfile; myfile.open(oss2.str(), ios::out|ios::trunc);
   //srand(123456);
   int maxi = 10;
   for(int i=0;i<maxi;i++){
       restart();
       for (int j=0; j<veces; j++)
       {
           inicializacion();
           while(!parar)
           {
               suc_sig = temporizacion();
               suceso(suc_sig,data, j);
           }
       }
       // informe();
       progress_bar(float(i+1)/float(maxi));
       avg_finforme(myfile);
       data << "\n\n";
   }

   cout << "\n";
}
