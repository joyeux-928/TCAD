void placementAndBuffering(Circuit& circuit) {
    // 初始化坐标
    for (auto& cell : circuit.cells) {
        cell.x = cell.col;
        cell.y = cell.row;
    }

    // 迭代优化单元格位置
    bool changed;
    do {
        changed = false;
        for (auto& cell : circuit.cells) {
            double idealX = 0.0;
            for (int conn : cell.connections) {
                idealX += circuit.cells[conn].x;
            }
            idealX /= cell.connections.size();

            if (abs(cell.x - idealX) > 1e-6) {
                cell.x = idealX;
                changed = true;
            }
        }
    } while (changed);

    // 插入缓冲器
    for (int i = 0; i < circuit.cells.size(); ++i) {
        Cell& cell = circuit.cells[i];
        for (int conn : cell.connections) {
            Cell& connectedCell = circuit.cells[conn];
            double wireLength = abs(cell.x - connectedCell.x) + abs(cell.y - connectedCell.y);

            // 如果线长超过限制，插入缓冲器
            if (wireLength > 1.0) { // 假设最大线长为1
                Buffer buffer;
                buffer.x = (cell.x + connectedCell.x) / 2;
                buffer.y = (cell.y + connectedCell.y) / 2;
                circuit.buffers.push_back(buffer);
            }
        }
    }
}