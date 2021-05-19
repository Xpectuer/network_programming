
IP=$1
PORT=$2
LOAD=$3
load_func(){
		for i in {1..$LOAD} 
	do
		./client $IP:$PORT >> load.log &
	done	
}

load_func

