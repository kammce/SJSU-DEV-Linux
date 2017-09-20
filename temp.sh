
#TERM=""
#OPENOCD=""

#if [[ "$SYS" != "Darwin" || "$SYS" != "Linux" ]]; then
#	echo "I have determined you are running $SYS."
#	echo ""
#	echo "There are no pre-compiled packages available for your system. Would"
#	echo "you like to compile them from source? (This will take extra time.)"
#	echo ""
#	echo -n "[y/n]: "
#	read DECIDE
#	DECIDE=$(echo $DECIDE | tr '[:upper:]' '[:lower:]')
#	if [ $DECIDE == 'yes' || $DECIDE == 'y' ]; then
#		BUILD_SRC='yes'
#	fi
#fi

