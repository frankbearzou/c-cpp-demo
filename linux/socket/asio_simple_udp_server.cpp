#include <iostream>
#include <algorithm>
#include <cctype>
#include <thread>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

void system_call(char *arg)
{
	system(arg);
}

void main()
{
	io_service service;
	ip::udp::socket upd_sockets(service);
	ip::udp::endpoint server_addr(ip::address::from_string("127.0.0.1"), 8000);

	upd_sockets.open(server_addr.protocol());
	upd_sockets.bind(server_addr);

	while (1)
	{
		char data[1024] = { 0 };
		ip::udp::endpoint conn_addr;
		upd_sockets.receive_from(buffer(data, 1024), conn_addr);
		cout << data << endl;
		thread t1(system_call, data);
		t1.join();
		transform(begin(data), end(data), begin(data), toupper);
		upd_sockets.send_to(buffer(data), conn_addr);
	}

	system("pause");
}
