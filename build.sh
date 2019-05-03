#!/bin/bash

git clone git://git.osmocom.org/libosmocore
cd libosmocore
autoreconf -fi
./configure
make -j5
sudo make install

cd ..
git clone git://git.osmocom.org/libosmo-abis
cd libosmo-abis
autoreconf -fi
./configure
make -j5
sudo make install

cd ..
git clone git://git.osmocom.org/libosmo-netif
cd libosmo-netif
autoreconf -fi
./configure
make -j5
sudo make install

cd ..
git clone git://git.osmocom.org/libosmo-sccp
cd libosmo-sccp
autoreconf -fi
./configure
make -j5
sudo make install

cd ..
git clone git://git.osmocom.org/libsmpp34
cd libsmpp34
autoreconf -fi
./configure
make
sudo make install

cd ..
git clone git://git.osmocom.org/osmo-ggsn
cd osmo-ggsn
autoreconf -fi
./configure
make -j5
sudo make install

cd ..
git clone git://git.osmocom.org/openbsc
cd openbsc/openbsc
autoreconf -fi
./configure --enable-smpp --enable-osmo-bsc --enable-nat
make -j5
sudo make install $$ sudo ldconfig

cd ../..
git clone git://git.osmocom.org/osmo-bts.git
cd osmo-bts
autoreconf -fi
./configure --enable-trx
make
sudo make install
sudo ldconfig

cd ..
git clone git://git.osmocom.org/osmo-sip-connector.git
cd osmo-sip-connector
autoreconf -fi
./configure
make
sudo make install $$ sudo ldconfig
