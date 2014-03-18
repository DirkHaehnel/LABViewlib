void  init_simulation_interface( void);
void  close_simulation_interface( void);

int  irq_handle( void);

void  write_adress( unsigned char aktuelle_adress);
void  write_data( unsigned char  aktuelle_daten_low, unsigned char  aktuelle_daten_high);
void  read_data( unsigned char  *aktuelle_daten_low, unsigned char  *aktuelle_daten_high);
void  write_ctrl( int rw_leitung, int ioen_leitung, int  reset_leitung);

void   E710_write_command( unsigned char  address,
							unsigned short int data);
void  E710_read_command( unsigned char  address,
							unsigned short int *data);
void  E710_ldac_command( void);


