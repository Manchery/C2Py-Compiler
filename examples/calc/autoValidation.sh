while true; do
    python expGen.py > exp.in
    ret=$?
    if [ $ret -ne 0 ]; then
        echo FailGenExp
        continue
    fi

    python groundTruth.py < exp.in > truth.out
    ./calc < exp.in > calc.out

    if diff truth.out calc.out; then
        echo Accepted
        cat exp.in
    else
        echo WrongAnswer
        break
    fi
done