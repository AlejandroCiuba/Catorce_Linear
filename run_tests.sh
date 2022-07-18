# Runs all test files
#!/bin/bash

DATA_TYPES=("NO_TYPE" "CHAR" "INT32" "INT64" "FLOAT" "DOUBLE")
TEST_FILES=("test")

for test in ${TEST_FILES[@]}
do
    for type in ${!DATA_TYPES[@]}
    do 
        echo "RUNNING ${test} FOR TYPE ${DATA_TYPES[$type]}..."
        ./${test} -d ${type} -n 5
        echo ""
    done

    echo "TESTS RAN FOR TEST FILE ${test}..."
    
done

echo ""
echo "FINISHED TESTS, RUNNING VALGRIND..."

for test in ${TEST_FILES[@]}
do
    # Make the test_file
    make

    for type in ${!DATA_TYPES[@]}
    do 
        echo "RUNNING VALGRIND FOR ${test} FOR TYPE ${DATA_TYPES[$type]}..."
        valgrind --leak-check=full --track-origins=yes -s ./${test} -d ${type} -n 5
        echo ""
    done

    echo "TESTS RAN FOR TEST FILE ${test}..."
    
done

echo "DONE."