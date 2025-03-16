double calculateEntanglementEntropy(const Circuit& circuit) {
    double entropy = 0.0;
    int totalWires = 0;

    for (int i = 0; i < circuit.rows - 1; ++i) {
        unordered_map<int, int> wireOrderPrev;
        unordered_map<int, int> wireOrderNext;

        // 统计当前行和下一行的线序
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

        // 计算纠缠熵
        for (const auto& [wire, orderPrev] : wireOrderPrev) {
            if (wireOrderNext.find(wire) != wireOrderNext.end()) {
                int orderNext = wireOrderNext[wire];
                entropy += pow(orderPrev - orderNext, 2);
                totalWires++;
            }
        }
    }

    return entropy / totalWires; // 归一化纠缠熵
}