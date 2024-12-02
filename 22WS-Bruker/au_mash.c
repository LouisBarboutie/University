/*
    Description:
    This au programm waits for the given period and then halts the rotation

    Author: Louis-Hendrik Barboutie
*/

AUERR = au_mash()
QUITMSG("=== AU program mash finished ===")

int au_mash(int day, int hour, int minute){

    int t_sleep;

    // expt

    t_sleep = 60 * ( minute + 60 * ( hour + 24 * day ); // get the sleep time in seconds
    sleep(t_sleep);
    MASH

    return EXIT_SUCCESS;
}