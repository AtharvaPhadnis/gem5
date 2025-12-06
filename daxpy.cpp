#include <iostream>
#include <vector>
#include <cstdlib>

int main() {
    // 500,000 elements * 16 bytes (2 arrays) = ~8 MB Data
    // This is significantly larger than a 256KB L2 cache.
    const size_t N = 500000; 
    const double alpha = 1.1;

    std::cout << "Running DAXPY with N=" << N << std::endl;
    std::cout << "Total Working Set: " << (N * 2 * sizeof(double)) / 1024.0 << " KB" << std::endl;

    // Allocate memory
    std::vector<double> X(N);
    std::vector<double> Y(N);

    // Initialize arrays (Access 1: Write Misses)
    // We do this to ensure pages are allocated and warm up TLBs slightly
    for (size_t i = 0; i < N; ++i) {
        X[i] = static_cast<double>(i);
        Y[i] = static_cast<double>(N - i);
    }

    std::cout << "Initialization complete. Starting computation loop..." << std::endl;

    // DAXPY Loop (Access 2: Read/Write Hits & Misses)
    // This linear scan will thrash the L2 cache since the data doesn't fit.
    for (size_t i = 0; i < N; ++i) {
        Y[i] = alpha * X[i] + Y[i];
    }

    // Prevent compiler optimization
    std::cout << "Done. Check val: " << Y[0] << " " << Y[N-1] << std::endl;

    return 0;
}
