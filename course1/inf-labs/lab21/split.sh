#!/bin/sh

function isNumber {
	re='^[0-9]+$'
	if [[ $1 =~ $re ]] ; then
		echo 1
	else
		echo 0
	fi
}

function alpha {
	case "$1" in
	0) echo a ;;
	1) echo b ;;
	2) echo c ;;
	3) echo d ;;
	4) echo e ;;
	5) echo f ;;
	6) echo g ;;
	7) echo h ;;
	8) echo i ;;
	9) echo j ;;
	10) echo k ;;
	11) echo l ;;
	12) echo m ;;
	13) echo n ;;
	14) echo o ;;
	15) echo p ;;
	16) echo q ;;
	17) echo r ;;
	18) echo s ;;
	19) echo t ;;
	20) echo u ;;
	21) echo v ;;
	22) echo w ;;
	23) echo x ;;
	24) echo y ;;
	25) echo z ;;
	esac
}

function printData {
	echo error = $error
	echo a = $a
	echo b = $b
	echo l = $l
	echo p = $p
	echo name = $name
}

function genName {
	num=$1
	res=0
	i=0
	name=""
	suffix=""
	len_suff=$(( ($1 + 25) / 26 ))
	len_pref=$(( $a - $len_suff ))

	while [ $i -lt $len_pref ] ; do
		name="${name}a"
		i=$(( $i + 1 ))
	done

	while [[ $num -ne 0 ]] ; do
		suffix="$suffix$( alpha $(( $num % 26 )) )"
		num=$(( $num / 26 ))
	done
	suffix=$( echo $suffix | rev )
	name="$name$suffix"
	echo $name
}

error=None
a=None
b=None
l=None
p=None

while [ -n "$1" ]
do
	case "$1" in
	-a)
		if [ -n $2 ] && [ $(isNumber $2) -eq 1 ] && [ $2 -ne 0 ] && [ $2 -lt 64 ]
		then
			a="$2"
			shift
		elif [ $2 -gt 64 ] ; then
			echo "split: suffix is too long (max 64)"
			exit
		else
			echo split: $2: illegal suffix length
			exit
		fi
		;;
	-b)
		if [ -n $2 ] && [ $(isNumber $2) -eq 1 ] && [ $2 -ne 0 ] && [ $l = None ] && [ $p = None ]; then
			b="$2"
			shift
		elif [ $l != None ] || [ $p != None ] ; then
			echo usage: split [-a sufflen] [-b byte_count] [-l line_count] [-p pattern] [file [prefix]]
			exit
		else
			echo split: $2: illegal byte count
			exit
		fi
		;;
	-l)
		if [ -n $2 ] && [ $(isNumber $2) -eq 1 ] && [ $2 -ne 0 ] && [ $b = None ] && [ $p = None ] ; then
			l="$2"
			shift
		elif [ $b != None ] || [ $p != None ] ; then
			echo usage: split [-a sufflen] [-b byte_count] [-l line_count] [-p pattern] [file [prefix]]
			exit
		else
			echo split: $2: illegal line count
			exit
		fi
		;;
	-p)
		if [ -n $2 ] && [ $2 != "" ] && [ $b = None ] && [ $l = None ]; then
			p=$2
			shift
		elif [ $b != None ] || [ $l != None ] ; then
			echo usage: split [-a sufflen] [-b byte_count] [-l line_count] [-p pattern] [file [prefix]]
			exit
		else
			echo split: $2: illegal regexp
			exit
		fi
		;;
	--)
		shift
		break ;;
	esac
	shift
done

if [ $b = None ] && [ $l = None ] && [ $p = None ] ; then
	l=1000
fi

if [ $a = None ] ; then
	a=2
fi

if [ -n "$1" ] ; then
	if ! [ -e "$1" ] ; then
		echo split: $1: No such file or directory
		exit
	fi
	if [ -d "$1" ] ; then
		echo split: $1: Is a directory
		exit
	fi
	if ! [ -r "$1" ] ; then
		echo split: $1: Permission denied
		exit
	fi
	exec 0< $1
	shift
fi

if [ -n "$1" ] ; then
	name="$1"
else
	name="x"
fi


cnt_lines=$l
cnt_files=0

while [ $l != None ] && read line ; do

	if [ $cnt_files -eq $(( 26 ** $a )) ] ; then
		echo split: too many files
		exit
	fi

	if [ $cnt_lines -eq $l ] ; then
		cnt_lines=0
		file="$name$(genName $cnt_files)"
		cnt_files=$(( $cnt_files + 1 ))
		rm -rf $file
		touch $file
	fi

	echo $line >> $file
	cnt_lines=$(( $cnt_lines + 1 ))

done

if [ $l != None ] && [ -n $line ] ; then
	if [ $cnt_lines -eq $l ] ; then
		cnt_lines=0
		file="$name$(genName $cnt_files)"
		cnt_files=$(( $cnt_files + 1 ))
		rm -rf $file
		touch $file
	fi
	printf "$line" >> $file
	cnt_lines=$(( $cnt_lines + 1 ))
fi

cnt_bytes=$b

while [ $b != None ] && read -n 1 letter ; do

	if [[ $cnt_files -eq $(( 26 ** $a )) ]] ; then
		echo split: too many files
		exit
	fi

	if [ $cnt_bytes -eq $b ] ; then
		cnt_bytes=0
		file="$name$(genName $cnt_files)"
		cnt_files=$(( $cnt_files + 1 ))
		rm -rf $file
		touch $file
	fi

	if [[ $letter = "" ]] ; then
		printf "\n" >> $file
	else
		printf "$letter" >> $file
	fi
	cnt_bytes=$(( $cnt_bytes + 1 ))

done

it=1


while [ $p != None ] && read line ; do
	
	if [[ $cnt_files -eq $(( 26 ** $a )) ]] ; then
		echo split: too many files
		exit
	fi
	if [[ $line =~ $p ]] || [ $it -eq 1 ] ; then
		file="$name$(genName $cnt_files)"
		cnt_files=$(( $cnt_files + 1 ))
		rm -rf $file
		touch $file
		echo $line >> $file
		if [ $it -eq 1 ] ; then
			it=$(( $it + 1 ))
		fi
	else
		echo $line >> $file
	fi
done

if [ $p != None ] && [ -n $line ] ; then
	if [[ $line =~ $p ]] ; then
		file="$name$(genName $cnt_files)"
		cnt_files=$(( $cnt_files + 1 ))
		rm -rf $file
		touch $file
		printf "$line" >> $file
		if [ $it -eq 1 ] ; then
			it=$(( $it + 1 ))
		fi
	else
		printf "$line" >> $file
	fi
fi
