if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    export DISPLAY=:99.0
    sh -e /etc/init.d/xvfb start
    sleep 3 # give xvfb some time to start
    eval "${MATRIX_EVAL}"

    mkdir -p third_party
    cd third_party
    
    mkdir -p gtest_build
    cd gtest_build
    cmake /usr/src/gtest
    make -j2
    cd ..
    
    git clone https://github.com/rvt/libdxfrw.git libdxfrw
    cd libdxfrw
    cmake .
    make -j2
    cd ../..
    
    mkdir -p build
    cd build
    cmake -DRUN_CLANG_TIDY_PY=/usr/bin/run-clang-tidy-6.0.py -DCHECKS_SOURCE_BRANCH=${TRAVIS_BRANCH} ..
    make -j2

    if [[ $? != 0 ]]; then
        echo "Compilation failed"
        exit 1;
    fi

    ./unittest/lcunittest

    #if [[ $TRAVIS_EVENT_TYPE == "pull_request" ]]; then
    #    echo "Doing partial style check"
    #    make diffstylecheck > stylecheck.out
    #else 
    #    make stylecheck > stylecheck.out
    #fi

    #if [[ -n $(grep "warning: " stylecheck.out) ]] || [[ -n $(grep "error: " stylecheck.out) ]]; then
    #    echo "Style checks failed"
    #    echo ""
    #    exit 1;
    #else
    #    echo -e "\033[1;32m\xE2\x9C\x93 passed:\033[0m $1";
    #fi
    
    #cat stylecheck.out
fi
