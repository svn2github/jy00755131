echo on 

cd dist
del *.*
cd ..
cd bin
@echo "*************************************"
@echo "***** waiting for some minutes ******"
@echo "*************************************"
instmk --verbose --file c:\setup\eprom\setup.insp

