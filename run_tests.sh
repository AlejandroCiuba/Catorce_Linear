# Runs all test files
#!/bin/bash

DATA_TYPES=("NO_TYPE" "CHAR" "INT32" "INT64" "FLOAT" "DOUBLE")
TEST_FILES=("test")

for test in ${TEST_FILES[@]}
do
    # Make the test_file
    make

    for type in ${!DATA_TYPES[@]}
    do 
        echo "RUNNING ${test} FOR TYPE ${DATA_TYPES[$type]}..."
        ./${test} -d ${type} -n 5
        echo ""
    done

    echo""
    echo "TESTS RAN FOR TEST FILE ${test}..."

done

echo "\nDONE."