#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PUERTO 5003                    //numero de puerto asignado al servidor
#define COLA_CLIENTES 5                  //tamano cola espera para clientes
#define TAM_BUFFER 100

void *RecibirProveedor(void*);
void *AbrirSucursal(void*);
void *EncenderCarbon(void*);
void *PrepararHamburguesas(void*);
void *HacerLimpieza(void*);
void *VerDinero(void*);
void *CobrarClientes(void*);
void *Horario(void*);
void servidor(int a);

sem_t AbreSucursal;
sem_t Proveedores;
sem_t Carbon;
sem_t Hamburguesas;
sem_t Limpieza;
sem_t CorteFinal;
sem_t TomaPedidos;
sem_t Hora;

int proveer=0;
int limpio=0;
int turno=0;
int cobrar=0;
int clientes;
int ham;
int precio=120;
int total;
int caja;
int sillasLlenas=0;
int carbonListo=0;
int pedido=0;
int hamburguesa=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3=PTHREAD_MUTEX_INITIALIZER;

int main(){
	srand(time(NULL));
	clientes=rand()%10;
	
	pthread_t Mercancia;
	pthread_t Abrir;
	pthread_t CarbonH;
	pthread_t Hamburguesa1;
	pthread_t Hamburguesa2;
	pthread_t Hamburguesa3;
	pthread_t Limpiar;
	pthread_t Ordenes1;
	pthread_t Ordenes2;
	pthread_t Dinero;
	pthread_t Horario_hilo;

	sem_init(&Hora,0,1);	
	sem_init(&AbreSucursal, 0, 0);
	sem_init(&Proveedores, 0, 0);
	sem_init(&Carbon, 0, 0);
	sem_init(&Hamburguesas, 0, 0);
	sem_init(&Limpieza, 0, 0);
	sem_init(&CorteFinal, 0, 0);
	sem_init(&TomaPedidos, 0, 0);

	
	if(pthread_create(&Mercancia, NULL, RecibirProveedor, NULL)){
	 	printf("problema en la creación del hilo\n");
	 	exit(EXIT_FAILURE);
	}	
	
	if(pthread_create(&Abrir, NULL, AbrirSucursal, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&CarbonH, NULL,EncenderCarbon, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Hamburguesa1, NULL, PrepararHamburguesas, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Hamburguesa2, NULL, PrepararHamburguesas, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Hamburguesa3, NULL, PrepararHamburguesas, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Limpiar, NULL, HacerLimpieza, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Ordenes1, NULL, CobrarClientes, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Ordenes2, NULL, CobrarClientes, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Dinero, NULL, VerDinero, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&Horario_hilo, NULL, Horario, NULL)){
		printf("problema en la creación del hilo\n");
		exit(EXIT_FAILURE);
	}
	
	
	

	if(pthread_join(Mercancia, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}	
	if(pthread_join(Abrir, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(CarbonH, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Hamburguesa1, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Hamburguesa2, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Hamburguesa3, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Limpiar, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Ordenes1, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Ordenes2, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Dinero, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(Horario_hilo, NULL)){
		printf("Problema al crear el enlace con otro hilo\n");
		exit(EXIT_FAILURE);
	}


	sem_destroy (&AbreSucursal);
	sem_destroy (&Proveedores);
	sem_destroy (&Carbon);
	sem_destroy (&Hamburguesas);
	sem_destroy (&Limpieza);
	sem_destroy (&CorteFinal);
	sem_destroy (&TomaPedidos);
	
	

	 exit(0);
}


void *AbrirSucursal(void* x){
while(1){
	//printf("verificasndo apertura");
	sem_wait(&AbreSucursal);
	printf("Necesitamos que la sucursal este limpia\n");
	if(limpio==0){
		printf("La sucursal aún no ha sido limpiada, aun no podemos abrir\n");
		for(int i=0;i<clientes;i++){
			printf("Llenando silla #%d que esta fuera de sucursal\n",i);
			sillasLlenas=sillasLlenas+1;
		}
		sem_post(&Limpieza);
	}
	else{
		printf("La sucursal ya esta limpia, vamos a abrir\n");
		sem_post(&TomaPedidos);
	}
}
}



void *HacerLimpieza(void* x){
	sem_wait(&Limpieza);
	printf("Necesitamos que la sucursal este limpia\n");
	printf("Listo, ya esta limpio\n");
	limpio=1;
	sem_post(&AbreSucursal);
}

void *RecibirProveedor(void* x){
	sem_wait(&Proveedores);
	printf("Recibiendo proveedores para preparar hamburguesas\n");
	proveer=1;
	printf("Proveedor recibido\n");
	sem_post(&TomaPedidos);
}

void *EncenderCarbon(void* x){
	sem_wait(&Carbon);
	printf("Preparando carbon\n");
	carbonListo=1;
	printf("El carbon ya está listo padrino\n");
	sem_post(&Hamburguesas);
}

void *CobrarClientes(void* x){
while(1){
	sem_wait(&TomaPedidos);
	printf("Empezando a tomar pedido de %d clientes\n",clientes);
	//printf("El cliente %d ordeno &d hamburguesas\n",clientes);
	printf("Checando ingredientes\n");
	if(proveer==0){
		printf("Aun no hay ingredientes necesarios\n");
		sem_post(&Proveedores);
	}
	else{
		printf("Si tenemos los ingredientes necesaios\n");
		printf("Creando pedidos y cobrando\n");
		for(int i=0;i<clientes;i++){
			total=0;
			pedido=pedido+1;
			ham=rand()%4;
			total= ham*precio;
			caja=caja+total;
			printf("Cobrando pedido #%d, pidio %d hamburguesas, con un total de: %d\n",pedido,ham,total);
			printf("El total en caja es: %d\n",caja);
		}
		sem_post(&Hamburguesas);
	}
	}
}

void *PrepararHamburguesas(void* x){

	sem_wait(&Hamburguesas);
	printf("Checando pedidos\n");
	if(pedido==0){
		printf("Aun no hay pedidos\n");
		sem_post(&TomaPedidos);
	}
	printf("Checando carbon\n");
	if(carbonListo==0){
		printf("El carbon no esta listo\n");
		sem_post(&Carbon);
	}
	else{
		printf("Estamos listos para servir\n");
		
		//for(int i=0;i<pedido;i++){
			pedido=pedido+1;
			printf("Preprando pedido #%d \n",pedido);
			hamburguesa=hamburguesa+1;
		//}
		sem_post(&CorteFinal);
	}
}


void *VerDinero(void* x){
	sem_wait(&CorteFinal);
	printf("Todos los clientes fueron atendidos\n");
	printf("El PÍD %li haciendo el corte final de la sucursal\n",pthread_self());

	printf("Cerrando\n");
}

void *Horario(void* x){
	sem_wait(&Hora);	
	int a=1;
	while(1){	
	time_t t = time(NULL);
	struct tm tiempoLocal = *localtime(&t);
	char fechaHora[10];
	char *formato = "%H%M";
	int bytesEscritos = strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);
	int hora = atoi(fechaHora);	
	printf("La hora es:%i\n",hora);
	sleep(2);
	if(hora>=1000&&hora<2300)
		{
			if(a==1)
			{
			//a=0;
			sem_post(&AbreSucursal);
			}
			printf("El local esta abierto");
			
		}
	else
		{
			printf("el local esta cerrado\n");
			a=0;
			if(a==0)
			{	
			printf("El total fue de:%i\n",caja);
			servidor(caja);
			//sockets
			}
			exit(-1);
		}
	}
	
}

void servidor(int a){
	int tamano_direccion, sockfd, cliente_sockfd;
   struct sockaddr_in direccion_servidor; //estructura que almacena direccion
   char leer_mensaje[TAM_BUFFER];
   int num;

   //Se configura la direccion IPv4 para configurar el socket
   memset (&direccion_servidor, 0, sizeof (direccion_servidor));
   direccion_servidor.sin_family = AF_INET;
   direccion_servidor.sin_port = htons(PUERTO);
   direccion_servidor.sin_addr.s_addr = INADDR_ANY;

   //Creacion de las estructuras necesarias para el manejo de un socket
   printf("Creando Socket ....\n");
   if ( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0){
	perror("Ocurrio un problema en la creacion del socket");
	exit(1);
   }

   //Configuracion del socket mediante la direccion IPv4
   printf("Configurando socket ...\n");
   if (bind(sockfd, (struct sockaddr *) &direccion_servidor,
	sizeof(direccion_servidor)) < 0){
	perror ("Ocurrio un problema al configurar el socket");
	exit(1);
   }

   //Define el socket listo para aceptar peticiones
   printf ("Estableciendo la aceptacion de clientes...\n");
   if ( listen(sockfd, COLA_CLIENTES) < 0){
	perror("Ocurrio un problema al crear la cola de aceptar peticiones de los clientes");
	exit(1);
   }

   //Selecciona un cliente de la cola de conexiones establecidas
   //se crea un nuevo descriptor de socket para el manejo
   //de la nueva conexion. Apartir de este punto se debe
   //utilizar el nuevo descriptor de socket
   printf ("En espera de peticiones de conexión ...\n");
   if ( (cliente_sockfd = accept(sockfd, NULL, NULL)) < 0){
	perror("Ocurrio algun problema al atender a un cliente");
	exit(1);
   }

   //Transferencia de datos entre cliente y Servidor
   printf ("Se aceptó un cliente, atendiendolo");
   if (read (cliente_sockfd,&leer_mensaje, TAM_BUFFER) < 0){
	perror ("Ocurrio algun problema al recibir datos del cliente");
	exit(1);
   }
   printf ("El cliente nos envio el siguiente mensaje: \n %s \n",leer_mensaje);
      
int total = atoi(leer_mensaje);
printf("\n\nEl total de las dos sucursales fue de:%i\n\n",total+a);

close (sockfd);
   

}











