#!/bin/sh

# Wrap tshark to hide the root warning
if [ ! -f /usr/bin/tshark-real ]; then
    mv /usr/bin/tshark /usr/bin/tshark-real
    cat <<'EOF' > /usr/bin/tshark
#!/bin/sh
# Merge stderr into stdout, then filter out the root warning line
exec /usr/bin/tshark-real "$@" 2>&1 | grep -v 'Running as user "root" and group "root"'
EOF
    chmod +x /usr/bin/tshark
fi

#!/bin/sh
ldconfig
mv /json.hpp ./json.hpp 2>/dev/null

g++ -std=c++11 -Wall -fexceptions -g -pthread -I inc -I libs/inc -c src/main.cpp -o bin/main.o
g++ -std=c++11 -Wall -fexceptions -g -pthread -I inc -I libs/inc -c src/svgts.cpp -o bin/svgts.o
g++ -std=c++11 -Wall -fexceptions -g -pthread -I libs/inc -c libs/src/rmqbroker.cpp -o bin/rmqbroker.o

g++ -std=c++11 -o acquisition_module bin/rmqbroker.o bin/svgts.o bin/main.o -lpthread -lSimpleAmqpClient
LD_LIBRARY_PATH=/usr/local/lib/x86_64-linux-gnu/ ./acquisition_module