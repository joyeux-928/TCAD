void topologyInitialization(Circuit& circuit) {
    bool changed;
    do {
        changed = false;
        for (int row = 0; row < circuit.rows; ++row) {
            for (int i = 0; i < circuit.cols; ++i) {
                for (int j = i + 1; j < circuit.cols; ++j) {
                    // 交换单元格
                    swap(circuit.cells[row * circuit.cols + i], circuit.cells[row * circuit.cols + j]);
                    double newEntropy = calculateEntanglementEntropy(circuit);

                    // 如果纠缠熵减小，则保留交换
                    if (newEntropy < calculateEntanglementEntropy(circuit)) {
                        changed = true;
                    } else {
                        swap(circuit.cells[row * circuit.cols + i], circuit.cells[row * circuit.cols + j]);
                    }
                }
            }
        }
    } while (changed);
}