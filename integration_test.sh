#!/bin/bash 

function clean {
  echo 'cleaning...'
  rm textin
  rm textout
  rollback
}

function rollback {
  echo 'rolling back...'

  if [ -f 'test_output.bmp' ]; then
    rm test_output.bmp
  fi
}

function compile {
  echo 'compiling...'
  cc -o textin textin.c
  cc -o textout textout.c
}

clean
compile

echo 'testing textin creates a file'
./textin test_input.bmp test_output.bmp 'fred'
if [ -f 'test_output.bmp' ]; then
  echo 'PASS: test_output.bmp was created'
else
  echo 'FAIL: test_output.bmp was not created'
fi

rollback

echo 'testing textout outputs message from a file'
retreived_text=$(./textout fixture_image.bmp)
if [ "$retreived_text" == "Hello World" ]; then
  result="PASS"
else
  result="FAIL"
fi
echo $result $retreived_text " was output"

rollback

echo "Completed"
