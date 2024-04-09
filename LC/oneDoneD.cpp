auto better = [&](int j1, int j2, int i) -> bool {// j1 is a better than j2 for i
                                                  // To be implemented
};
deque<pi> dk;                  // starting from p.F, p.S will be the best trasition point
auto add = [&](int j, int ir) {// i is defined in [0, ir)
    while (dk.size() >= 1 && better(j, dk.back().S, dk.back().F)) dk.pop_back();
    if (dk.empty()) dk.EB(0, j);
    else {
        int lb = dk.back().F + 1, rb = ir - 1;
        while (lb <= rb) {
            int mb = (lb + rb) / 2;
            if (better(j, dk.back().S, mb)) rb = mb - 1;
            else lb = mb + 1;
        }
        if (lb < ir) dk.EB(lb, j);
    }
};
auto qry = [&](int i) {
    assert(dk.size());
    while (dk.size() >= 2 && dk[1].F <= i) dk.pop_front();
    return dk[0].S;
};
