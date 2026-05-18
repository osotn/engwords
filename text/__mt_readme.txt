git clone -b spi_test git@bitbucket.org:ajaxsystems/sigma_lab_dualos_533.git 
 2002  cd sigma_lab_dualos_533/
 2004  cd sourcecode/
 2005  cd alkaid/
 2006  cd project/
 2008  sudo cp /bin/sh /bin/sh_
 2009  sudo rm /bin/sh
 2010  sudo ln -s /bin/bash /bin/sh
       export ARCH=arm
 2011  export CROSS_COMPILE=arm-linux-gnueabihf-
 2017  export PATH=/home/sotnikov-a/gitspace/sigma_lab_dualos_533/toolchain/gcc-linaro-9.1.0-2019.11-x86_64_arm-linux-gnueabihf/bin:$PATH
 2018  ./setup_config.sh configs/defconfigs/ipc-rtos-smplh_i6e_spinand.glibc-9.1.0-022a.256.smplh.qfn128_defconfig
 2019  make image -j8
 2036  sudo cp -r  ./image/output/images/* /srv/tftp/
 2037  sudo unlink /bin/sh
 2038  sudo cp /bin/sh_ /bin/sh
