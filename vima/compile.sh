#!/bin/bash
HOME="/home/garren/Documents/Projects/video_simulations"
SIM_HOME=$HOME"/OrCS"
PIN_HOME=$SIM_HOME"/trace_generator/pin"
SINUCA_TRACER_HOME=$SIM_HOME"/trace_generator/extras/pinplay/bin/intel64/sinuca_tracer.so"
CODE_HOME=$HOME"/orcs_validation/vima"
COMP_FLAGS="-O2 -DNOINLINE -static"
SIZES=(4 8)
SIZES_MATMUL=(4)
MATMUL_VECTOR_SIZE=(64)

cd $CODE_HOME

if [ ! -d "exec" ]; then
	mkdir -p "exec"
fi

if [ ! -d "traces" ]; then
	mkdir -p "traces"
fi

for i in *.c
do 
    rm exec/${i%.c}.out
    g++ $i $COMP_FLAGS -o exec/${i%.c}.out
    echo "Working on "$i
    # if [[ ${i%.c} == matmul* ]]; then
    #     for j in "${SIZES_MATMUL[@]}";
    #     do
    #         for k in "${MATMUL_VECTOR_SIZE[@]}";
    #         do
    #             echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} ${k} &> nohup.out &"
    #             #nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &
    #             $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} ${k}
    #         done
    #     done
    # else
    if [[ ${i%.c} == vecsum* ]]; then
    	for j in "${SIZES[@]}";
    	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &"
            #nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &
            $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j}
    	done
    fi
done
