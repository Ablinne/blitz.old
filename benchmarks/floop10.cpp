
// floop10 generated by makeloops.py Tue Jun 28 15:54:20 2011

#include <blitz/vector2.h>
#include <blitz/array.h>
#include <random/uniform.h>
#include <blitz/benchext.h>

#ifdef BZ_HAVE_VALARRAY
 #define BENCHMARK_VALARRAY
#endif

#ifdef BENCHMARK_VALARRAY
#include <valarray>
#endif

BZ_NAMESPACE(blitz)
extern void sink();
BZ_NAMESPACE_END

BZ_USING_NAMESPACE(blitz)
BZ_USING_NAMESPACE(std)

#if defined(BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
 #define floop10_f77 floop10_f77_
 #define floop10_f77overhead floop10_f77overhead_
 #define floop10_f90 floop10_f90_
 #define floop10_f90overhead floop10_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define floop10_f77 floop10_f77__
 #define floop10_f77overhead floop10_f77overhead__
 #define floop10_f90 floop10_f90__
 #define floop10_f90overhead floop10_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define floop10_f77 FLOOP10_F77
 #define floop10_f77overhead FLOOP10_F77OVERHEAD
 #define floop10_f90 FLOOP10_F90
 #define floop10_f90overhead FLOOP10_F90OVERHEAD
#endif

extern "C" {
  void floop10_f77(const int& N, float* x, float* a, float* b, float* c, const float& u);
  void floop10_f77overhead(const int& N, float* x, float* a, float* b, float* c, const float& u);
  void floop10_f90(const int& N, float* x, float* a, float* b, float* c, const float& u);
  void floop10_f90overhead(const int& N, float* x, float* a, float* b, float* c, const float& u);

}

void VectorVersion(BenchmarkExt<int>& bench, float u);
void ArrayVersion(BenchmarkExt<int>& bench, float u);
void ArrayVersion_unaligned(BenchmarkExt<int>& bench, float u);
void ArrayVersion_misaligned(BenchmarkExt<int>& bench, float u);
void ArrayVersion_index(BenchmarkExt<int>& bench, float u);
void doTinyVectorVersion(BenchmarkExt<int>& bench, float u);
void F77Version(BenchmarkExt<int>& bench, float u);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, float u);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, float u);
#endif

const int numSizes = 20;
const int Nmax=1<<(numSizes-1);
const int tvNmax=7;
const bool runvector=false; // no point as long as Vector is Array<1>

int main()
{
    int numBenchmarks = 6;
    if (runvector) numBenchmarks++;
#ifdef BENCHMARK_VALARRAY
    numBenchmarks++;
#endif
#ifdef FORTRAN_90
    numBenchmarks++;
#endif

    BenchmarkExt<int> bench("floop10: $x = u+$a+$b+$c", numBenchmarks);

    bench.setNumParameters(numSizes);

    Array<int,1> parameters(numSizes);
    Array<long,1> iters(numSizes);
    Array<double,1> flops(numSizes);

    for (int i=0; i < numSizes; ++i)
    {
      parameters(i) = Nmax>>i;
      iters(i) = 50000000L / parameters(i);
	
        if (iters(i) < 2)
            iters(i) = 2;
        flops(i) = 3 * parameters(i);
    }

    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setOpsPerIteration(flops);
    bench.setDependentVariable("flops");
    bench.beginBenchmarking();

    float u = 0.39123982498157938742;


    ArrayVersion(bench, u);
    ArrayVersion_unaligned(bench, u);
    ArrayVersion_misaligned(bench, u);
    ArrayVersion_index(bench, u);
    doTinyVectorVersion(bench, u);
    F77Version(bench, u);
#ifdef FORTRAN_90
    F90Version(bench, u);
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench, u);
#endif

    if(runvector)
      VectorVersion(bench, u);

    bench.endBenchmarking();

    bench.saveMatlabGraph("floop10.m");
    return 0;
}

template<class T>
void initializeRandomDouble(T* data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

template<class T>
void initializeRandomDouble(valarray<T>& data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

void VectorVersion(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Vector<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Vector<float> x(N);
        initializeRandomDouble(x.data(), N);
        Vector<float> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<float> b(N);
        initializeRandomDouble(b.data(), N);
        Vector<float> c(N);
        initializeRandomDouble(c.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = u+a+b+c;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<float,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<float,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<float,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<float,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = u+a+b+c;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion_index(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1> (indexexpr.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<float,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<float,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<float,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<float,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = u+a(tensor::i)+b(tensor::i)+c(tensor::i);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_unaligned(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1> (unal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<float,1> xfill(N+1);
    Array<float,1> x(xfill(Range(1,N)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<float,1> afill(N+1);
    Array<float,1> a(afill(Range(1,N)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<float,1> bfill(N+1);
    Array<float,1> b(bfill(Range(1,N)));
    initializeRandomDouble(b.dataFirst(), N);

    Array<float,1> cfill(N+1);
    Array<float,1> c(cfill(Range(1,N)));
    initializeRandomDouble(c.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = u+a+b+c;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_misaligned(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1> (misal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<float,1> xfill(N+4);
    Array<float,1> x(xfill(Range(0,N+0-1)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<float,1> afill(N+4);
    Array<float,1> a(afill(Range(1,N+1-1)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<float,1> bfill(N+4);
    Array<float,1> b(bfill(Range(2,N+2-1)));
    initializeRandomDouble(b.dataFirst(), N);

    Array<float,1> cfill(N+4);
    Array<float,1> c(cfill(Range(3,N+3-1)));
    initializeRandomDouble(c.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = u+a+b+c;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


template<int N>
void TinyVectorVersion(BenchmarkExt<int>& bench, float u)
{
        cout << bench.currentImplementation() << ": N = " << N << endl;

        const int sz = bench.getParameter();
        assert(N==sz);
                           
        long iters = bench.getIterations();

        TinyVector<float,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        TinyVector<float,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<float,N> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        TinyVector<float,N> c(N);
        initializeRandomDouble(c.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = u+a+b+c;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench, u);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench, float u)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench, float u)
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

  TinyVectorVersion< 1<<tvNmax >(bench, u);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        long iters = bench.getIterations();

        valarray<float> x(N);
        initializeRandomDouble(x, N);
        valarray<float> a(N);
        initializeRandomDouble(a, N);
        valarray<float> b(N);
        initializeRandomDouble(b, N);
        valarray<float> c(N);
        initializeRandomDouble(c, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = u+a+b+c;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  sink();
	}
        bench.stopOverhead();
    }

    bench.endImplementation();
}
#endif

void F77Version(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        float* x = new float[N];
        initializeRandomDouble(x, N);
        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* b = new float[N];
        initializeRandomDouble(b, N);
        float* c = new float[N];
        initializeRandomDouble(c, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop10_f77(N, x, a, b, c, u);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop10_f77overhead(N, x, a, b, c, u);

        bench.stopOverhead();

        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        float* x = new float[N];
        initializeRandomDouble(x, N);
        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* b = new float[N];
        initializeRandomDouble(b, N);
        float* c = new float[N];
        initializeRandomDouble(c, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop10_f90(N, x, a, b, c, u);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop10_f90overhead(N, x, a, b, c, u);

        bench.stopOverhead();
        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;

    }

    bench.endImplementation();
}
#endif

