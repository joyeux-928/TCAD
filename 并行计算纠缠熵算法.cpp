#include <thread>
#include <mutex>

double calculateEntanglementEntropyParallel(const Circuit& circuit) {
    double entropy = 0.0;
    int totalWires = 0;

    vector<thread> threads;
    mutex entropyMutex;

    auto worker = [&](int startRow, int endRow) {
        double localEntropy = 0.0;
        int localWires = 0;

        for (int i = startRow; i < endRow; ++i) {
            unordered_map<int, int> wireOrderPrev;
            unordered_map<int, int> wireOrderNext;

            for (const auto& cell : circuit.cells) {
                if (cell.row == i) {
                    for (int conn : cell.connections) {
                        wireOrderPrev[conn] = cell.col;
                    }
                } else if (cell.row == i + 1) {
                    for (int conn : cell.connections) {
                        wireOrderNext[conn] = cell.col;
                    }
                }
            }

            for (const auto& [wire, orderPrev] : wireOrderPrev) {
                if (wireOrderNext.find(wire) != wireOrderNext.end()) {
                    int orderNext = wireOrderNext[wire];
                    localEntropy += pow(orderPrev - orderNext, 2);
                    localWires++;
                }
            }
        }

        lock_guard<mutex> lock(entropyMutex);
        entropy += localEntropy;
        totalWires += localWires;
    };

    int numThreads = thread::hardware_concurrency();
    int rowsPerThread = circuit.rows / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? circuit.rows : (i + 1) * rowsPerThread;
        threads.emplace_back(worker, startRow, endRow);
    }

    for (auto& t : threads) {
        t.join();
    }

    return entropy / totalWires;
}