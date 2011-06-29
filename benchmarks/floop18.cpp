
// floop18 generated by makeloops.py Wed Jun 29 17:36:24 2011

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
 #define floop18_f77 floop18_f77_
 #define floop18_f77overhead floop18_f77overhead_
 #define floop18_f90 floop18_f90_
 #define floop18_f90overhead floop18_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define floop18_f77 floop18_f77__
 #define floop18_f77overhead floop18_f77overhead__
 #define floop18_f90 floop18_f90__
 #define floop18_f90overhead floop18_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define floop18_f77 FLOOP18_F77
 #define floop18_f77overhead FLOOP18_F77OVERHEAD
 #define floop18_f90 FLOOP18_F90
 #define floop18_f90overhead FLOOP18_F90OVERHEAD
#endif

extern "C" {
  void floop18_f77(const int& N, float* x, float* a, float* b, const float& u, const float& v);
  void floop18_f77overhead(const int& N, float* x, float* a, float* b, const float& u, const float& v);
  void floop18_f90(const int& N, float* x, float* a, float* b, const float& u, const float& v);
  void floop18_f90overhead(const int& N, float* x, float* a, float* b, const float& u, const float& v);

}

void VectorVersion(BenchmarkExt<int>& bench, float u, float v);
void ArrayVersion(BenchmarkExt<int>& bench, float u, float v);
void ArrayVersion_unaligned(BenchmarkExt<int>& bench, float u, float v);
void ArrayVersion_misaligned(BenchmarkExt<int>& bench, float u, float v);
void ArrayVersion_index(BenchmarkExt<int>& bench, float u, float v);
void doTinyVectorVersion(BenchmarkExt<int>& bench, float u, float v);
void F77Version(BenchmarkExt<int>& bench, float u, float v);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, float u, float v);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, float u, float v);
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

    BenchmarkExt<int> bench("floop18: $x = (u+$a)*(v+$b)", numBenchmarks);

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
    float v = 0.39123982498157938742;


    ArrayVersion(bench, u, v);
    ArrayVersion_unaligned(bench, u, v);
    ArrayVersion_misaligned(bench, u, v);
    ArrayVersion_index(bench, u, v);
    doTinyVectorVersion(bench, u, v);
    F77Version(bench, u, v);
#ifdef FORTRAN_90
    F90Version(bench, u, v);
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench, u, v);
#endif

    if(runvector)
      VectorVersion(bench, u, v);

    bench.endBenchmarking();

    bench.saveMatlabGraph("floop18.m");
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

void VectorVersion(BenchmarkExt<int>& bench, float u, float v)
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


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
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


  void ArrayVersion(BenchmarkExt<int>& bench, float u, float v)
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


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
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


  void ArrayVersion_index(BenchmarkExt<int>& bench, float u, float v)
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


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a(tensor::i))*(v+b(tensor::i));;
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

  void ArrayVersion_unaligned(BenchmarkExt<int>& bench, float u, float v)
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


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
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

  void ArrayVersion_misaligned(BenchmarkExt<int>& bench, float u, float v)
{
    bench.beginImplementation("Array<T,1> (misal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<float,1> xfill(N+3);
    Array<float,1> x(xfill(Range(0,N+0-1)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<float,1> afill(N+3);
    Array<float,1> a(afill(Range(1,N+1-1)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<float,1> bfill(N+3);
    Array<float,1> b(bfill(Range(2,N+2-1)));
    initializeRandomDouble(b.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
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
void TinyVectorVersion(BenchmarkExt<int>& bench, float u, float v)
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


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench, u, v);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench, float u, float v)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench, float u, float v)
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

  TinyVectorVersion< 1<<tvNmax >(bench, u, v);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, float u, float v)
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


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
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

void F77Version(BenchmarkExt<int>& bench, float u, float v)
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
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop18_f77(N, x, a, b, u, v);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop18_f77overhead(N, x, a, b, u, v);

        bench.stopOverhead();

        delete [] x;
        delete [] a;
        delete [] b;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, float u, float v)
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


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop18_f90(N, x, a, b, u, v);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop18_f90overhead(N, x, a, b, u, v);

        bench.stopOverhead();
        delete [] x;
        delete [] a;
        delete [] b;

    }

    bench.endImplementation();
}
#endif

