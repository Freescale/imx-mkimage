#!/bin/bash

BL32="tee.bin"

let fit_off=$1

# We dd flash.bin to 33KB "0x8400" offset, so need minus 0x8400
let uboot_sign_off=$((fit_off - 0x8400 + 0x3000))
let uboot_size=$(stat --printf="%s" u-boot-nodtb.bin)
let uboot_load_addr=0x40200000

let atf_sign_off=$((uboot_sign_off + uboot_size))
let atf_load_addr=0x910000
let atf_size=$(stat --printf="%s" bl31.bin)

if [ ! -f $BL32 ]; then
	let tee_size=0x0
	let tee_sign_off=$((atf_sign_off + atf_size))
else
	let tee_size=$(stat --printf="%s" tee.bin)

	let tee_sign_off=$((atf_sign_off + atf_size))
	let tee_load_addr=0xfe000000
fi

let last_sign_off=$((tee_sign_off + tee_size))
let last_size=$((tee_size))
let last_load_addr=$((uboot_load_addr + uboot_size))

uboot_size=`printf "0x%X" ${uboot_size}`
uboot_sign_off=`printf "0x%X" ${uboot_sign_off}`
uboot_load_addr=`printf "0x%X" ${uboot_load_addr}`

tee_size=`printf "0x%X" ${tee_size}`
tee_sign_off=`printf "0x%X" ${tee_sign_off}`
tee_load_addr=`printf "0x%X" ${tee_load_addr}`

atf_size=`printf "0x%X" ${atf_size}`
atf_sign_off=`printf "0x%X" ${atf_sign_off}`
atf_load_addr=`printf "0x%X" ${atf_load_addr}`

echo ${uboot_load_addr} ${uboot_sign_off} ${uboot_size}
echo ${atf_load_addr} ${atf_sign_off} ${atf_size}

if [ ${tee_size} != 0 ]
then
	echo ${tee_load_addr} ${tee_sign_off} ${tee_size}
fi

cnt=0
for dtname in $*
do
	if [ ${cnt} != 0 ]
	then
		let fdt${cnt}_size=$(stat --printf="%s" $dtname)

		let fdt${cnt}_sign_off=$((last_sign_off))
		let fdt${cnt}_load_addr=$((last_load_addr))
		let last_size=$((fdt${cnt}_size))

		fdt_size=`printf "0x%X" ${last_size}`
		fdt_sign_off=`printf "0x%X" ${last_sign_off}`
		fdt_load_addr=`printf "0x%X" ${last_load_addr}`

		let last_sign_off=$((last_sign_off + fdt${cnt}_size))
		let last_load_addr=$((last_load_addr + fdt${cnt}_size))

		echo ${fdt_load_addr} ${fdt_sign_off} ${fdt_size}
	fi

	cnt=$((cnt+1))
done
