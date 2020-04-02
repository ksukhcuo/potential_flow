//
//  2 dimentional Potential Flow solved by finite difference method
//  using Successive Over-Relaxation method (SOR)
//

#include <iostream>
#include <cmath>
#include <vector>
#include "Gnuplot.h"

int main(int argc, const char *argv[]){
    
    const double PI = std::acos(-1.0);
    constexpr int NX = 50;
    constexpr int NY = 50;
    constexpr int Na = 19;
    constexpr int Nb = 30;
    const double dx = 5.0/(double)(NX);
    const double dy = 5.0/(double)(NY);
    const double dx2 = dx*dx;
    const double dy2 = dy*dy;
    const double dx_2 = 1.0/dx2;
    const double dy_2 = 1.0/dy2;
    const double Dx = 0.5*dx2/(dx2+dy2);
    const double Dy = 0.5*dy2/(dx2+dy2);
    const double Dxy = 0.5/(dx_2+dy_2);
    double omega = 1.6;
    constexpr int itermax = 1e+6;
    constexpr double errmax = 1.0e-6;

    std::vector<double> x(NX), y(NY);
    for(int i=0; i<NX; i++)  x[i] = ((double)i + 0.5)*dx;
    for(int i=0; i<NY; i++)  y[i] = ((double)i + 0.5)*dy;

    std::vector<double> f(NX*NY);
    for(int i=0; i<NX*NY; i++) f[i] = 0.0;


    std::vector<double> fw(NY), qs(NX), fe(NY), qn(NX);
    for(int i=0; i<NX; i++){
        qs[i] = 0.0;
        qn[i] = 0.0;
    }
    for(int j=0; j<NY; j++){
        fw[j] = 0.0;
        fe[j] = 1.0;
    }

    int iter = 0;
    while(iter < itermax){
        iter++;
        double err = 0.0;
        for(int j=0; j<NY; j++){
            for(int i=0; i<NX; i++){
                int m = i + NX*j;
                int mw = m-1; int me = m+1; int ms = m-NX; int mn = m+NX;
                double F = f[m];
                // corner of the rectangle
                if( m == 0 )     f[m] += omega*( Dx*(fw[j]*2.0-f[m]+f[me]) + Dy*(-qs[i]*dy+f[m]+f[mn]) - f[m] );
                else if(m==NX-1) f[m] += omega*( Dx*(f[mw]+fe[j]*2.0-f[m]) + Dy*(-qs[i]*dx+f[m]+f[mn]) - f[m] );
                else if(m==NX*(NY-1)) f[m] += omega*( Dx*(fw[j]*2.0-f[m]+f[me]) + Dy*(f[ms]+qn[i]*dy+f[m]) - f[m] );
                else if(m==NX*NY-1)   f[m] += omega*( Dx*(f[mw]+fe[j]*2.0-f[m]) + Dy*(f[ms]+qn[i]*dy+f[m]) - f[m] );
                // edge of the rectangle
                else if(i == 0)  f[m] += omega*( Dx*(fw[j]*2.0-f[m]+f[me]) + Dy*(f[ms]+f[mn]) - f[m] );
                else if(i==NX-1) f[m] += omega*( Dx*(f[mw]+fe[j]*2.0-f[m]) + Dy*(f[ms]+f[mn]) - f[m] );
                else if(j == 0)  f[m] += omega*( Dx*(f[mw]+f[me]) + Dy*(-qs[i]*dy+f[m]+f[mn]) - f[m] );
                else if(j==NY-1) f[m] += omega*( Dx*(f[mw]+f[me]) + Dy*(f[ms]+qn[i]*dy+f[m]) - f[m] );
                // around the centeral square (normal component of velocity is zero)
                else if(j==Na && i>Na && i<Nb) f[m] += omega*( Dx*(f[mw]+f[me]) + Dy*(f[ms]+f[m]) - f[m] );
                else if(j==Nb && i>Na && i<Nb) f[m] += omega*( Dx*(f[mw]+f[me]) + Dy*(f[m]+f[mn]) - f[m] );
                else if(i==Na && j>Na && j<Nb) f[m] += omega*( Dx*(f[mw]+f[m]) + Dy*(f[ms]+f[mn]) - f[m] );
                else if(i==Nb && j>Na && j<Nb) f[m] += omega*( Dx*(f[m]+f[me]) + Dy*(f[ms]+f[mn]) - f[m] );
                // inside of the central square
                else if(i>Na && i<Nb && j>Na && j<Nb) ;
                // else
                else             f[m] += omega*( Dx*(f[mw]+f[me]) + Dy*(f[ms]+f[mn]) - f[m] );
                if(std::fabs(f[m]-F) > err) err = std::fabs(f[m]-F);
            }
        }
        if(err < errmax) break;
    }
    std::cout << "iteration:" << iter << std::endl;
    
    Gnuplot *gnuplot = new Gnuplot("gnuplot");
    gnuplot->SetAxisLabel("x", "x");
    gnuplot->SetAxisLabel("y", "y");
    gnuplot->Injection("set cbrange [0:1]\n");
    gnuplot->Injection("set size square\n");
    gnuplot->Mesh("splot", x, y, f);
    gnuplot->Flush();

    std::vector<double> u(NX*NY), v(NX*NY);
    for(int i=0; i<NX*NY; i++){
        u[i] = 0.0;
        v[i] = 0.0;
    }
    for(int j=0; j<NY; j++){
        for(int i=0; i<NX; i++){
            int m = i + NX*j;
            int mw = m-1; int me = m+1; int ms = m-NX; int mn = m+NX;

            if(i == 0)       u[m] = (f[me] - 2.0*fw[j] + f[m])/(2.0*dx);
            else if(i==NX-1) u[m] = (2.0*fe[j] - f[m] - f[mw])/(2.0*dx);
            else if(i>Na && i<Nb && j>Na && j<Nb) ;
            else if(i==Na && j>Na && j<Nb) u[m] = (f[m] - f[mw])/(2.0*dx);
            else if(i==Nb && j>Na && j<Nb) u[m] = (f[me] - f[m])/(2.0*dx);
            else             u[m] = (f[me] - f[mw])/(2.0*dx);

            if(j == 0)       v[m] = (f[mn] - f[m] + qs[i]*dx)/(2.0*dx);
            else if(j==NY-1) v[m] = (f[m] + qn[i]*dx - f[ms])/(2.0*dx);
            else if(i>Na && i<Nb && j>Na && j<Nb) ;
            else if(j==Na && i>Na && i<Nb) v[m] = (f[m] - f[ms])/(2.0*dx);
            else if(j==Nb && i>Na && i<Nb) v[m] = (f[mn] - f[m])/(2.0*dx);
            else             v[m] = (f[mn] - f[ms])/(2.0*dx);
        }
    }
    Gnuplot* gnuvector = new Gnuplot();
    gnuvector->Injection("set size square\n");
    gnuvector->Injection("unset key\n");
    gnuvector->SetGraphRange("x", 0.0, 5.0);
    gnuvector->SetGraphRange("y", 0.0, 5.0);
    gnuvector->Injection("set cbrange [0:0.35]\n");
    gnuvector->Injection("set style arrow 1 size graph 0.02,20 filled nohead linewidth 1\n");
    gnuvector->Injection("set style arrow 2 size graph 0.02,20 filled nohead linewidth 1\n");
    gnuvector->Injection("set style arrow 3 size graph 0.02,20 filled nohead linewidth 1\n");
    gnuvector->Injection("set style arrow 4 size graph 0.02,20 filled nohead linewidth 1\n");
    gnuvector->Injection("set arrow 1 from 2,2 to 2,3 arrowstyle 1\n");
    gnuvector->Injection("set arrow 2 from 2,3 to 3,3 arrowstyle 2\n");
    gnuvector->Injection("set arrow 3 from 3,3 to 3,2 arrowstyle 3\n");
    gnuvector->Injection("set arrow 4 from 3,2 to 2,2 arrowstyle 4\n");
    gnuvector->Vector("plot", x, y, u, v, 1.0);
    gnuvector->Flush();
    int n;
    std::cout << "Please enter 0 to stop." << std::endl;
    std::cin >> n;
    delete gnuplot;
    delete gnuvector;

    return 0;
}