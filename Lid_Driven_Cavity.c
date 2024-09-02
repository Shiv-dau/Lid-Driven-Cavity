#include<stdio.h>
#include<math.h>
#include<conio.h>

int main()
{
    FILE *fp;
    FILE *P;
    FILE *S;
    fp=fopen("result.dat","w");
    P=fopen("central_u.dat","w");
    S=fopen("central_v.dat","w");
    int i,j,m,n,tp,re,iteration;
    float dx,dy,x,y,b;
    printf("enter re= ");
    scanf("%d",&re);
     printf("\nenter m= ");
    scanf("%d",&m);
     printf("\nenter n= ");
    scanf("%d",&n);

    tp=(m-2)*(n-2);            //total no. of interior points
    dx=1.0/(m-1);
    dy=1.0/(n-1);

    b=(dx/dy);               //   (dx,dy:-grid size)

    float str_fun[m][n],str_fun_previous[m][n],omega[m][n],omega_previous[m][n],u[m][n],v[m][n];     /// s:-stream function  w:-vorticity  

    float str_fun_err=0.0,omega_err=0.0;

    iteration=0;

    //boundary condition initialization

    for(j=0;j<n;j++)
    {
         for(i=0;i<m;i++)
         {
             v[i][j]=0.0;
             str_fun[i][j]=0.0;
             if(j==(n-1)){
              u[i][j]=1.0;
             }
             else{
               u[i][j]=0.0;
            }
         }
    }
    for(j=0;j<n;j++)
     {
          for(i=0;i<m;i++)
          {
              if(j==0){                                                       //   bottom wall
              omega[i][j]=(2.0/dy*dy)*(str_fun[i][j]-str_fun[i][j+1]);
              }
              else if(i==0){                                                   //  left wall
                omega[i][j]=(2.0/dx*dx)*(str_fun[i][j]-str_fun[i+1][j]);
              }
              else if(i==(m-1)){                                              //   right wall
                omega[i][j]=(2.0/dx*dx)*(str_fun[i][j]-str_fun[i-1][j]);
              }
              else if(j==(n-1)){                                              //   top wall
                omega[i][j]=(2.0/dy*dy)*(str_fun[i][j]-str_fun[i][j-1])-((2.0/dy)*u[i][j]);
              }
              else{
                    omega[i][j]=0.0;
                  } 
          }
     }
     do
     {
         for(j=0;j<n;j++)
         {
             for(i=0;i<m;i++)
             {
                 str_fun_previous[i][j]=str_fun[i][j];
                 omega_previous[i][j]=omega[i][j];
             }
         }
         //SOLVING STREAM FUNCTION FOR INTERIOR POINTS
          for(j=1;j<(n-1);j++)
         {
             for(i=1;i<(m-1);i++)
             {
                 str_fun[i][j]=(1.0/(2*(1.0+b*b)))*(str_fun[i+1][j]+str_fun[i-1][j]+b*b*(str_fun[i][j+1]+str_fun[i][j-1])+dx*dx*omega[i][j]);
             }
         }

         //SOLVING VORTICITY FOR INTERIOR POINTS
         for(j=1;j<(n-1);j++)
         {
             for(i=1;i<(m-1);i++)
             {
                 omega[i][j] = (0.5/(1.0+pow(b,2)))*((1.0-(str_fun[i][j+1]-str_fun[i][j-1])*((b*re)/4.0))*omega[i+1][j]+ (1.0+(str_fun[i][j+1]-str_fun[i][j-1])*((b*re)/4.0))*omega[i-1][j]
                     + (1.0+(str_fun[i+1][j]-str_fun[i-1][j])*(re/(4.0*b)))*(pow(b,2)*omega[i][j+1])+ (1.0-(str_fun[i+1][j]-str_fun[i-1][j])*(re/(4.0*b)))*(pow(b,2)*omega[i][j-1]));;
             }
         }
         // UPDATE VORTICITY AT BOUNDARIES USING PREVIOUS INTERIOR POINTS
          for(j=0; j<n; j++)
       {
           omega[0][j]=(-2*(str_fun[1][j]-str_fun[0][j]))/(dx*dx);
           omega[m-1][j]=(-2*(str_fun[m-2][j]-str_fun[m-1][j]))/(dx*dx);
       }
           for(i=0; i<m; i++)
       {
            omega[i][0]=(-2*(str_fun[i][1]-str_fun[i][0]))/(dy*dy);
            omega[i][n-1]=(-2*(str_fun[i][n-2]-str_fun[i][n-1]+dy))/(dy*dy);
       }
            //ERROR CALCULATION
            str_fun_err=0.0;
            omega_err=0.0;
            for(j=1;j<(n-1);j++)
         {
             for(i=1;i<(m-1);i++)
        {

              str_fun_err=str_fun_err+pow((str_fun[i][j]-str_fun_previous[i][j]),2.0);
              omega_err=omega_err+pow((omega[i][j]-omega_previous[i][j]),2.0);
          }
      }
      str_fun_err=sqrt(str_fun_err/tp);
      omega_err=sqrt(omega_err/tp);

      printf("iteration=%d\t",iteration);
      printf("error_psi=%.9lf\terror_omega=%.9lf\n",str_fun_err,omega_err);
      iteration++;
    }
    while(str_fun_err>pow(10,-6) || omega_err>pow(10,-6));

     //UPDATING VELOCITIES

         for (j=1;j<(n-1);j++)
         {
            for(i=1;i<(m-1);i++)
            {
             u[i][j]=(str_fun[i][j+1]-str_fun[i][j-1])/(2*dy);         //   FROM DEFINITION OF STREAM FUNCTION
             v[i][j]=(str_fun[i+1][j]-str_fun[i-1][j])/(-2.0*dx);
            }
         }
    fprintf(fp,"ZONE I=%d, J=%d\n",m,n);
for(i = 0; i < m; i++)
{
   x=i*dx;
for(j = 0; j < n; j++)
{
   y=j*dy;
fprintf(fp,"%f\t%f\t%f\t%f\t%f\t%f\n",x,y,u[i][j],str_fun[i][j],v[i][j],omega[i][j]);
}
}
for(i=0;i<m;i++)
fprintf(P,"%f \t %f \n",u[n/2][i],i*dy);
for(j=0;j<n;j++)
fprintf(S,"%lf \t %lf \n",j*dx,v[j][m/2]);
fclose(fp);
fclose(P);
fclose(S);
return 0;
}