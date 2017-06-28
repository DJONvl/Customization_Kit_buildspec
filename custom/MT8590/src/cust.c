
enum EMMC_MODE
{
   MODE_TLC = 0,
   MODE_SLC = 1
};

/* return the emmc cell mode.
*/
int get_emmc_mode()
{
   return (int)MODE_TLC; 
}