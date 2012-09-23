#include "mwc.hpp"

int main(int argc, char **argv)
{
	//MC::mwc<MC::product_mwc_dep> wc;
	MC::mwc<> wc;

	wc.load("./data");

    return 0;
}
