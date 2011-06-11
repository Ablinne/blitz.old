
// floop21 generated by makeloops.py Sat Jun 11 22:31:49 2011

#include <blitz/vector2.h>
#include <blitz/array.h>
#include <blitz/rand-uniform.h>
#include <blitz/benchext.h>

#ifdef BZ_HAVE_VALARRAY
 #define BENCHMARK_VALARRAY
#endif

#ifdef BENCHMARK_VALARRAY
#include <valarray>
#endif

BZ_USING_NAMESPACE(blitz)
BZ_USING_NAMESPACE(std)

#if defined(BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
 #define floop21_f77 floop21_f77_
 #define floop21_f77overhead floop21_f77overhead_
 #define floop21_f90 floop21_f90_
 #define floop21_f90overhead floop21_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define floop21_f77 floop21_f77__
 #define floop21_f77overhead floop21_f77overhead__
 #define floop21_f90 floop21_f90__
 #define floop21_f90overhead floop21_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define floop21_f77 FLOOP21_F77
 #define floop21_f77overhead FLOOP21_F77OVERHEAD
 #define floop21_f90 FLOOP21_F90
 #define floop21_f90overhead FLOOP21_F90OVERHEAD
#endif

extern "C" {
  void floop21_f77(const int& N, float* x, float* a, float* b, float* c, float* d);
  void floop21_f77overhead(const int& N, float* x, float* a, float* b, float* c, float* d);
  void floop21_f90(const int& N, float* x, float* a, float* b, float* c, float* d);
  void floop21_f90overhead(const int& N, float* x, float* a, float* b, float* c, float* d);

}

void VectorVersion(BenchmarkExt<int>& bench);
void ArrayVersion(BenchmarkExt<int>& bench);
void doTinyVectorVersion(BenchmarkExt<int>& bench);
void F77Version(BenchmarkExt<int>& bench);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench);
#endif

extern void sink();

const int numSizes = 20;
const int Nmax=1<<(numSizes-1);
const int tvNmax=7;

int main()
{
    int numBenchmarks = 5;
#ifndef BENCHMARK_VALARRAY
    numBenchmarks--;   // No  valarray
#endif
#ifndef FORTRAN_90
    numBenchmarks--;   // No fortran 90
#endif

    BenchmarkExt<int> bench("floop21: $x = $a*$b + $c*$d", numBenchmarks);

    bench.setNumParameters(numSizes);

    Vector<int> parameters(numSizes);
    Vector<long> iters(numSizes);
    Vector<double> flops(numSizes);

    for (int i=0; i < numSizes; ++i)
    {
      parameters(i) = Nmax>>i;
      iters(i) = 50000000L * (parameters(i)<4 ? 4/parameters(i) :1) / parameters(i);
	
        if (iters(i) < 2)
            iters(i) = 2;
        flops(i) = 2 * parameters(i);
    }

    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setFlopsPerIteration(flops);

    bench.beginBenchmarking();



    //VectorVersion(bench );
    ArrayVersion(bench );
    doTinyVectorVersion(bench );
    F77Version(bench );
#ifdef FORTRAN_90
    F90Version(bench );
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench );
#endif

    bench.endBenchmarking();

    bench.saveMatlabGraph("floop21.m");
    return 0;
}

template<class T>
void initializeRandomDouble(T data, int numElements, int stride = 1)
{
    static Random<Uniform> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

template<class T>
void initializeArray(T& array, int numElements)
{
    static Random<Uniform> rnd;

    for (size_t i=0; i < numElements; ++i)
        array[i] = rnd.random();
}

void ArrayVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << "Array<T,1>: N = " << N << endl;

        long iters = bench.getIterations();

        Array<float,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<float,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<float,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<float,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<float,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
	  asm("nop;nop;");
            x = a*b + c*d;
	  asm("nop;nop;");
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  asm("nop;nop;");
            sink();
	  asm("nop;nop;");
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}
  

template<int N>
void TinyVectorVersion(BenchmarkExt<int>& bench)
{
        cout << "Tinyvector<T, " << N << ">" << endl;
	bench.getParameter();

        long iters = bench.getIterations();

        TinyVector<float,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        TinyVector<float,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<float,N> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        TinyVector<float,N> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        TinyVector<float,N> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
	  asm("nop;nop;");
            x = a*b + c*d;
	  asm("nop;nop;");
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  asm("nop;nop;");
            sink();
	  asm("nop;nop;");
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench)
{
  bench.beginImplementation("TinyVector<T>");
  // can't run tinyvector with full length because meta-unrolling
  // kills compiler...
  int N=Nmax;
  while(N> 1<<tvNmax) {
   bench.getParameter();
   bench.getIterations();
   bench.skip();
   N>>=1;
  }

  TinyVectorVersion< 1<<tvNmax >(bench);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << "valarray<T>: N = " << N << endl;

        long iters = bench.getIterations();

        valarray<float> x(N);
        initializeRandomDouble(x, N);
        valarray<float> a(N);
        initializeRandomDouble(a, N);
        valarray<float> b(N);
        initializeRandomDouble(b, N);
        valarray<float> c(N);
        initializeRandomDouble(c, N);
        valarray<float> d(N);
        initializeRandomDouble(d, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
	  asm("nop;nop;");
            x = a*b + c*d;
	  asm("nop;nop;");
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  asm("nop;nop;");
	  sink();
	  asm("nop;nop;");
	}
        bench.stopOverhead();
    }

    bench.endImplementation();
}
#endif

void F77Version(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << "Fortran 77: N = " << N << endl;

        int iters = bench.getIterations();

        float* x = new float[N];
        initializeRandomDouble(x, N);
        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* b = new float[N];
        initializeRandomDouble(b, N);
        float* c = new float[N];
        initializeRandomDouble(c, N);
        float* d = new float[N];
        initializeRandomDouble(d, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop21_f77(N, x, a, b, c, d);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop21_f77overhead(N, x, a, b, c, d);

        bench.stopOverhead();

        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << "Fortran 90: N = " << N << endl;

        int iters = bench.getIterations();

        float* x = new float[N];
        initializeRandomDouble(x, N);
        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* b = new float[N];
        initializeRandomDouble(b, N);
        float* c = new float[N];
        initializeRandomDouble(c, N);
        float* d = new float[N];
        initializeRandomDouble(d, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop21_f90(N, x, a, b, c, d);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop21_f90overhead(N, x, a, b, c, d);

        bench.stopOverhead();
        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;

    }

    bench.endImplementation();
}
#endif

