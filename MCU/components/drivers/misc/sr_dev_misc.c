/*
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * file 	drv_misc.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2019年 06月 21日 星期五 10:50:47 CST
 *
 * brief    driver misc device 
 */


#include <stdio.h>
#include <string.h>
#include "drivers\misc\sr_dev_misc.h"
#include "bsp\include\drv_rs485.h"
#include "drivers\pgv\sr_pgv.h"
#include "bsp\include\drv_can.h"
#include "srtos\srdef.h"
#include "srtos\srconfig.h"
#include "bsp\include\drv_pin.h"
#include "drivers\motor\sr_motor_kinco.h"
#include "sr\app\navigation\include\sr_navigation_crl.h"


extern ST_CTL_PARA stCtlPara;



int32_t rmds_robo_module_init(void)
{
	uint8_t index = 0;
	int32_t ret = -1;
	sr_dev_comm_t dev_rmds_comm;
	uint8_t data = 0x55;
	
	dev_rmds_comm.dev_id = DEV_ID_RMDS_RESET;
	memset(&dev_rmds_comm.data, data, 8);
	index = 8;
	ret =  sr_can_write(SR_CAN1, dev_rmds_comm.dev_id, (uint8_t *)&dev_rmds_comm.data, index, 2);
	if (ret < 0){
		printf("failed to RMDS_RESET\r\n");
		return -1;
	}
	delay_ms(500);

	memset(&dev_rmds_comm, 0, sizeof(dev_rmds_comm));
	dev_rmds_comm.dev_id = DEV_ID_RMDS_MODE_CHOICE;
	dev_rmds_comm.data[0] = ENTER_PWM_VELOCITY_MODE;
	memset(&dev_rmds_comm.data[1], data, 7);
	index = 8;
	ret =  sr_can_write(SR_CAN1, dev_rmds_comm.dev_id, (uint8_t *)&dev_rmds_comm.data, index, 2);
	if (ret < 0){
		printf("failed to RMDS_MODE_CHOICE\r\n");
		return -1;
	}
	delay_ms(500);

	memset(&dev_rmds_comm, 0, sizeof(dev_rmds_comm));
	dev_rmds_comm.dev_id = DEV_ID_RMDS_CONFIG;
	dev_rmds_comm.data[0] = 0x01;
	memset(&dev_rmds_comm.data[1], data, 7);
	index = 8;
	ret =  sr_can_write(SR_CAN1, dev_rmds_comm.dev_id, (uint8_t *)&dev_rmds_comm.data, index, 2);
	if (ret < 0){
		printf("failed to RMDS_MODE_CHOICE\r\n");
		return -1;
	}
	delay_ms(500);	

	return 0;
}

int32_t smj_dsm8010_init(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_dsm8010_comm;

	/* Clear Errors */
	memset(&dev_dsm8010_comm, 0, sizeof(dev_dsm8010_comm));
	dev_dsm8010_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_L;
	dev_dsm8010_comm.data[index++] = 0x2B;
	dev_dsm8010_comm.data[index++] = 0x40;
	dev_dsm8010_comm.data[index++] = 0x60;
	dev_dsm8010_comm.data[index++] = 0x00;
	dev_dsm8010_comm.data[index++] = 0x80;

	ret =  sr_can_write(SR_CAN1, dev_dsm8010_comm.dev_id, (uint8_t *)&dev_dsm8010_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_L\r\n");
		return -1;
	}
	dev_dsm8010_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_R;
	ret =  sr_can_write(SR_CAN1, dev_dsm8010_comm.dev_id, (uint8_t *)&dev_dsm8010_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_R\r\n");
		return -2;
	}	
	delay_ms(500);

	/* reset */
	dev_dsm8010_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_L;
	ret =  sr_can_write(SR_CAN1, dev_dsm8010_comm.dev_id, (uint8_t *)&dev_dsm8010_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_L\r\n");
		return -3;
	}	
	dev_dsm8010_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_R;
	ret =  sr_can_write(SR_CAN1, dev_dsm8010_comm.dev_id, (uint8_t *)&dev_dsm8010_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_R\r\n");
		return -4;
	}	
	delay_ms(500);

	/* Speed Mode Slect */
	memset(&dev_dsm8010_comm, 0, sizeof(dev_dsm8010_comm));
	index = 0;
	dev_dsm8010_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_L;
	dev_dsm8010_comm.data[index++] = 0x2F;
	dev_dsm8010_comm.data[index++] = 0x60;
	dev_dsm8010_comm.data[index++] = 0x60;
	dev_dsm8010_comm.data[index++] = 0x00;
	dev_dsm8010_comm.data[index++] = 0xFE;
	ret =  sr_can_write(SR_CAN1, dev_dsm8010_comm.dev_id, (uint8_t *)&dev_dsm8010_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_L\r\n");
		return -5;
	}
	dev_dsm8010_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_R;
	ret =  sr_can_write(SR_CAN1, dev_dsm8010_comm.dev_id, (uint8_t *)&dev_dsm8010_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_R\r\n");
		return -6;
	}
	delay_ms(500);

	return 0;
}


//speed:RPM
int32_t mklm_speed_crl(uint16_t can_id, int16_t speed)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	int32_t tmp 	= 0;
	sr_dev_comm_t dev_speed_ctl_comm;

	if (speed < -3000){
		speed = -3000;
	}
	else if(speed > 3000) {
		speed = 3000;
	}
	tmp = (int32_t)speed*5000/3;

	/* Speed Mode Slect */
	memset(&dev_speed_ctl_comm, 0, sizeof(dev_speed_ctl_comm));
	dev_speed_ctl_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_L;
	dev_speed_ctl_comm.data[index++] = 0x23;
	dev_speed_ctl_comm.data[index++] = 0x41;
	dev_speed_ctl_comm.data[index++] = 0x23;
	dev_speed_ctl_comm.data[index++] = 0x00;
	dev_speed_ctl_comm.data[index++] = (unsigned char)(tmp&0xFF);
	dev_speed_ctl_comm.data[index++] = (unsigned char)((tmp>>8)&0xFF);
	dev_speed_ctl_comm.data[index++] = (unsigned char)((tmp>>16)&0xFF);
	dev_speed_ctl_comm.data[index++] = (unsigned char)((tmp>>24)&0xFF);
	ret =  sr_can_write(SR_CAN1, dev_speed_ctl_comm.dev_id, (uint8_t *)&dev_speed_ctl_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_L\r\n");
		return -1;
	}
	return 0;
}



int32_t mklm_embase_diff_init(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_mklm_comm;

	/* Speed Mode Slect */
	memset(&dev_mklm_comm, 0, sizeof(dev_mklm_comm));
	dev_mklm_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_L;
	dev_mklm_comm.data[index++] = 0x2B;
	dev_mklm_comm.data[index++] = 0x2B;
	dev_mklm_comm.data[index++] = 0x23;
	dev_mklm_comm.data[index++] = 0x00;
	dev_mklm_comm.data[index++] = 0x0B;
	ret =  sr_can_write(SR_CAN1, dev_mklm_comm.dev_id, (uint8_t *)&dev_mklm_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_L\r\n");
		return -1;
	}
	delay_ms(100);
	dev_mklm_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_R;
	ret =  sr_can_write(SR_CAN1, dev_mklm_comm.dev_id, (uint8_t *)&dev_mklm_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL_R\r\n");
		return -2;
	}

	mklm_speed_crl(DEV_ID_MIKELIMEI_WHEEL_L,0); //speed  = 0RMP
	delay_ms(100);
	mklm_speed_crl(DEV_ID_MIKELIMEI_WHEEL_R,0); //speed = 0RMP
	delay_ms(100);
	return 0;
}

int32_t mklm_embase_init(uint8_t embase_type)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_mklm_comm;

	/* Speed Mode Slect */
	memset(&dev_mklm_comm, 0, sizeof(dev_mklm_comm));
	if (embase_type == DIFF) {
		dev_mklm_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_L;
	} else if (embase_type == MEC){
		dev_mklm_comm.dev_id = DEV_ID_MKLM_MEC_WHEEL_A;
	}	
	dev_mklm_comm.data[index++] = 0x2B;
	dev_mklm_comm.data[index++] = 0x2B;
	dev_mklm_comm.data[index++] = 0x23;
	dev_mklm_comm.data[index++] = 0x00;
	dev_mklm_comm.data[index++] = 0x0B;
	ret =  sr_can_write(SR_CAN1, dev_mklm_comm.dev_id, (uint8_t *)&dev_mklm_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL\r\n");
		return -1;
	}
	delay_ms(100);

	if (embase_type == DIFF) {
		dev_mklm_comm.dev_id = DEV_ID_MIKELIMEI_WHEEL_R;
	} else if (embase_type == MEC){
		dev_mklm_comm.dev_id = DEV_ID_MKLM_MEC_WHEEL_B;
	}	
	ret =  sr_can_write(SR_CAN1, dev_mklm_comm.dev_id, (uint8_t *)&dev_mklm_comm.data, index, 2);
	if (ret < 0){
		printf("failed to MIKELIMEI_WHEEL\r\n");
		return -2;
	}
	delay_ms(100);

	if (embase_type == MEC) {
		dev_mklm_comm.dev_id = DEV_ID_MKLM_MEC_WHEEL_C;
		ret =  sr_can_write(SR_CAN1, dev_mklm_comm.dev_id, (uint8_t *)&dev_mklm_comm.data, index, 2);
		if (ret < 0){
			printf("failed to MKLM_MEC_WHEEL_C\r\n");
			return -3;
		}
		delay_ms(100);
		dev_mklm_comm.dev_id = DEV_ID_MKLM_MEC_WHEEL_D;
		ret =  sr_can_write(SR_CAN1, dev_mklm_comm.dev_id, (uint8_t *)&dev_mklm_comm.data, index, 2);
		if (ret < 0){
			printf("failed to MKLM_MEC_WHEEL_D\r\n");
			return -4;
		}
		delay_ms(100);
	}

	if (embase_type == DIFF) {
		mklm_speed_crl(DEV_ID_MIKELIMEI_WHEEL_L,0); //speed  = 0RMP
		delay_ms(100);
		mklm_speed_crl(DEV_ID_MIKELIMEI_WHEEL_R,0); //speed = 0RMP
		delay_ms(100);
	} else if (embase_type == MEC){
		mklm_speed_crl(DEV_ID_MKLM_MEC_WHEEL_A,0);//speed  = 0RMP
		delay_ms(100);
		mklm_speed_crl(DEV_ID_MKLM_MEC_WHEEL_B,0);//speed = 0RMP
		delay_ms(100);
		mklm_speed_crl(DEV_ID_MKLM_MEC_WHEEL_C,0);//speed = 0RMP
		delay_ms(100);
		mklm_speed_crl(DEV_ID_MKLM_MEC_WHEEL_D,0);//speed = 0RMP
		delay_ms(100);
	}
	return 0;
}




//HANG_TIAN_KE_JI Functions
int32_t htkj_servo_enable(uint16_t can_id)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_htkj_comm;

	memset(&dev_htkj_comm, 0, sizeof(dev_htkj_comm));
	dev_htkj_comm.dev_id = can_id;
	dev_htkj_comm.data[index++] = 0x00;
	dev_htkj_comm.data[index++] = 0x24;
	dev_htkj_comm.data[index++] = 0x00;
	dev_htkj_comm.data[index++] = 0x10;
	dev_htkj_comm.data[index++] = 0x00;
	dev_htkj_comm.data[index++] = 0x01;
	ret =  sr_can_write(SR_CAN1, dev_htkj_comm.dev_id, (uint8_t *)&dev_htkj_comm.data, index, 2);
	if (ret < 0){
		sr_kprintf("failed to htkj_servo_enable, ret=%d\r\n", ret);
		return -1;
	}
	return 0;
}

int32_t syntron_init(void)
{
	htkj_servo_enable(HTKJ_0);
	delay_ms(500);
	htkj_servo_enable(HTKJ_1);
	delay_ms(500);
	htkj_servo_enable(HTKJ_2);
	delay_ms(500);
	htkj_servo_enable(HTKJ_3);
	delay_ms(500);
	return 0;
}

int32_t tiger_init(void)
{
	mklm_speed_crl(OK_LEFT_WHEEL,0);//speed  = 0RMP
	delay_ms(100);
	mklm_speed_crl(OK_LEFT_WHEEL,0);//speed = 0RMP
	delay_ms(100);
	return 0;
}

int32_t curtis_handshake(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_handshake_comm;

	memset(&dev_handshake_comm, 0, sizeof(dev_handshake_comm));
	dev_handshake_comm.dev_id = 0;
	dev_handshake_comm.data[index++] = 0x01;
	ret =  sr_can_write(SR_CAN1, dev_handshake_comm.dev_id, (uint8_t *)&dev_handshake_comm.data, index, 2);
	if (ret < 0){
		printf("failed to curtis_handshake\r\n");
		return -1;
	}
	return 0;
}

int32_t curtis_heart_beat(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_heart_beat_comm;

	memset(&dev_heart_beat_comm, 0, sizeof(dev_heart_beat_comm));
	dev_heart_beat_comm.dev_id = 0x215;
	dev_heart_beat_comm.data[index++] = 0x05;
	ret =  sr_can_write(SR_CAN1, dev_heart_beat_comm.dev_id, (uint8_t *)&dev_heart_beat_comm.data, index, 2);
	if (ret < 0){
		printf("failed to curtis_heart_beat\r\n");
		return -1;
	}
	return 0;
}

int32_t curtis_manual_enable(void)
{
	uint8_t index 			= 0;
	int32_t ret 				= -1;
	uint16_t Speed_Acc		= 1500;
	uint16_t Speed_AccDown	= 200;
	sr_dev_comm_t dev_manual_comm;

	memset(&dev_manual_comm, 0, sizeof(dev_manual_comm));
	dev_manual_comm.dev_id = 0x213;
	dev_manual_comm.data[index++] = 0x04;
	dev_manual_comm.data[index++] = 0x00;
	dev_manual_comm.data[index++] = 0x00;
	dev_manual_comm.data[index++] = (uint8_t)((Speed_Acc>>8)&0xFF);  //设置为1000
	dev_manual_comm.data[index++] = (uint8_t)(Speed_Acc&0xFF);
	dev_manual_comm.data[index++] = (uint8_t)((Speed_AccDown>>8)&0xFF);
	dev_manual_comm.data[index++] = (uint8_t)(Speed_AccDown&0xFF);
	
	ret =  sr_can_write(SR_CAN1, dev_manual_comm.dev_id, (uint8_t *)&dev_manual_comm.data, index, 2);
	if (ret < 0){
		printf("failed to curtis_heart_beat\r\n");
		return -1;
	}
	return 0;
}


int32_t gulf_pt20_init(void)
{
	curtis_handshake();
	delay_ms(200);
	curtis_handshake();
	delay_ms(200);
	curtis_heart_beat();
	delay_ms(200);
	curtis_manual_enable();
	delay_ms(200);

	return 0;
}

int32_t sr_motor_driver_init(void)
{
	//CAN_Configuration(); 
	switch (stCtlPara.emMotorDriveType) {
		case RMDS:
			rmds_robo_module_init();
			break;
		
		case SMJ:
			smj_dsm8010_init();
			break;
		
		case MKLM:
			mklm_embase_init(stCtlPara.emBaseType);
			break;
		
		case KINCO:
			kinco_embase_init(stCtlPara.emBaseType);	
			break;
		
		case SYNTRON:
			syntron_init();
			break;
		
		case TIGER:
			tiger_init();
			break;
		
		case NANOTEC://ONLY PDO
			break;
			
		case GULF_PT20://ONLY PDO
			gulf_pt20_init();
			break;
			
		default:
			break;
	}
	return 0;
}


int32_t sr_l14_init(void)
{
	uint8_t index = 0;
	sr_dev_comm_t dev_l14_comm;

	dev_l14_comm.dev_id = DEV_ID_L14;
	dev_l14_comm.data[index++] = 0x01;
	dev_l14_comm.data[index++] = 0x00;

	return sr_can_write(SR_CAN1, dev_l14_comm.dev_id, (uint8_t *)&dev_l14_comm.data, index, 2);
}



int32_t jql_fork_encoder_init(void)
{
	uint8_t index = 0;
	sr_dev_comm_t dev_fork_encoder_comm;

	dev_fork_encoder_comm.dev_id = 0;
	dev_fork_encoder_comm.data[index++] = 0x01;
	dev_fork_encoder_comm.data[index++] = 0x20;
	return sr_can_write(SR_CAN1, dev_fork_encoder_comm.dev_id, (uint8_t *)&dev_fork_encoder_comm.data, index, 2);
}

int32_t tiean_motor_init(void)
{
	if (stCtlPara.ActionType == TIE_AN){
		kinco_err_clr(JUNGLEACTION_A); //顶升电机
		delay_ms(100);
		kinco_mode_select(JUNGLEACTION_A);
		delay_ms(100);
		kinco_pdo_enable(0x05);
		delay_ms(100);
		kinco_enable(JUNGLEACTION_A);
		delay_ms(100);
		kinco_spd_crl(JUNGLEACTION_A,0);
		delay_ms(100);
		jql_fork_encoder_init();
	}
	return 0;
}

int32_t aeropod_climb_motor_init(void)
{
	delay_ms(100);
	kinco_err_clr(Slide_Steer_WHEELC); delay_ms(100);
	kinco_err_clr(Slide_Steer_WHEELD); delay_ms(100);
	kinco_err_clr(Slide_Steer_WHEELE); delay_ms(100);	
	kinco_mode_select(Slide_Steer_WHEELC); delay_ms(100);	
	kinco_mode_select(Slide_Steer_WHEELD); delay_ms(100);	
	kinco_mode_select(Slide_Steer_WHEELE); delay_ms(100);	
	kinco_pdo_enable(0x03); delay_ms(100);	
	kinco_pdo_enable(0x04); delay_ms(100);	
	kinco_pdo_enable(0x05); delay_ms(100);	
	kinco_enable(Slide_Steer_WHEELC); delay_ms(100);	
	kinco_enable(Slide_Steer_WHEELD); delay_ms(100);	
	kinco_enable(Slide_Steer_WHEELE); delay_ms(100);	
	kinco_spd_crl(Slide_Steer_WHEELC,0);  delay_ms(100);	
	kinco_spd_crl(Slide_Steer_WHEELD,0); delay_ms(100);	
	kinco_spd_crl(Slide_Steer_WHEELE,0); delay_ms(100);
	
	return 0;
}

int32_t oasis300_jacking_motor_init(void)
{
	delay_ms(100);
	/* 顶升电机 */
	kinco_err_clr(JUNGLEACTION_A); delay_ms(100);
	/* 旋转电机 */
	kinco_err_clr(JUNGLEACTION_B); delay_ms(100);
	kinco_mode_select(JUNGLEACTION_A);  delay_ms(100);
	kinco_mode_select(JUNGLEACTION_B); delay_ms(100);
	kinco_pdo_enable(0x05); delay_ms(100);
	kinco_pdo_enable(0x06); delay_ms(100);	
	kinco_enable(JUNGLEACTION_A); delay_ms(100);	
	kinco_enable(JUNGLEACTION_B); delay_ms(100);	
	kinco_spd_crl(JUNGLEACTION_A,0); delay_ms(100);	
	kinco_spd_crl(JUNGLEACTION_B,0); delay_ms(100);	

	return 0;
}

int32_t oasis_tray_motor_init(void)
{
	delay_ms(100);
	/* 顶升电机 */
	kinco_err_clr(JUNGLEACTION_A); delay_ms(100);
	kinco_mode_select(JUNGLEACTION_A); delay_ms(100);
	kinco_pdo_enable(0x05); delay_ms(100);	
	kinco_enable(JUNGLEACTION_A); delay_ms(100);	
	kinco_spd_crl(JUNGLEACTION_A,0); delay_ms(100);

	return 0;	
}

int32_t pgv_down_stop(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_pgv_down_stop;

	memset(&dev_pgv_down_stop, 0, sizeof(dev_pgv_down_stop));
	dev_pgv_down_stop.dev_id = 0;
	dev_pgv_down_stop.data[index++] = 0x02;
	dev_pgv_down_stop.data[index++] = 0x07;
	ret =  sr_can_write(SR_CAN1, dev_pgv_down_stop.dev_id, (uint8_t *)&dev_pgv_down_stop.data, index, 2);
	if (ret < 0){
		printf("failed to pgv_down_stop\r\n");
		return ret;
	}
	return 0;
}


/****************************************************************************************
*************************	     Jungle PGV       ***************************************
******************************************************************************************/
uint8_t pgv_down_init(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_pgv_down_init;

	memset(&dev_pgv_down_init, 0, sizeof(dev_pgv_down_init));
	dev_pgv_down_init.dev_id = 0;
	dev_pgv_down_init.data[index++] = 0x01;
	dev_pgv_down_init.data[index++] = 0x07;
	ret =  sr_can_write(SR_CAN1, dev_pgv_down_init.dev_id, (uint8_t *)&dev_pgv_down_init.data, index, 2);
	if (ret < 0){
		printf("failed to fd122_clr_err\r\n");
		return ret;
	}
	return 0;
}

int32_t pgv_upper_stop(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_pgv_upper_stop;

	memset(&dev_pgv_upper_stop, 0, sizeof(dev_pgv_upper_stop));
	dev_pgv_upper_stop.dev_id = 0;
	dev_pgv_upper_stop.data[index++] = 0x02;
	dev_pgv_upper_stop.data[index++] = 0x08;
	ret =  sr_can_write(SR_CAN1, dev_pgv_upper_stop.dev_id, (uint8_t *)&dev_pgv_upper_stop.data, index, 2);
	if (ret < 0){
		sr_kprintf("failed to pgv_upper_stop, ret=%d\r\n", ret);
		return ret;
	}
	return 0;
}

int32_t pgv_upper_init(void)
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_pgv_upper_init;

	memset(&dev_pgv_upper_init, 0, sizeof(dev_pgv_upper_init));
	dev_pgv_upper_init.dev_id = 0;
	dev_pgv_upper_init.data[index++] = 0x01;
	dev_pgv_upper_init.data[index++] = 0x08;
	ret =  sr_can_write(SR_CAN1, dev_pgv_upper_init.dev_id, (uint8_t *)&dev_pgv_upper_init.data, index, 2);
	if (ret < 0){
		printf("failed to fd122_clr_err\r\n");
		return ret;
	}
	return 0;
}


int32_t jacking_down_cmd(void) 
{
	uint8_t index = 0;
	int32_t ret 	= -1;
	sr_dev_comm_t dev_pgv_upper_init;

	/* 控制顶升机构顶起来 */
	if (stCtlPara.EnableEU100 == 0x01) {
		memset(&dev_pgv_upper_init, 0, sizeof(dev_pgv_upper_init));
		dev_pgv_upper_init.dev_id = 0x301;
		dev_pgv_upper_init.data[index++] = 0xA0;
		dev_pgv_upper_init.data[index++] = 0x72;
		dev_pgv_upper_init.data[index++] = 0x11;
		dev_pgv_upper_init.data[index++] = 0x11;
		dev_pgv_upper_init.data[index++] = 0x64;
		dev_pgv_upper_init.data[index++] = 0x64;
		ret =  sr_can_write(SR_CAN1, dev_pgv_upper_init.dev_id, (uint8_t *)&dev_pgv_upper_init.data, index, 2);
		if (ret < 0){
			sr_kprintf("failed to jacking_down_cmd, ret=%d\r\n", ret);
			return ret;
		}

	} else {
		DIGITAL_OUT0 = 0;
		DIGITAL_OUT1 = 1;
	}
	return 0;
}



int32_t double_roller_init(void) 
{
	//初始化第一层辊轮，主要是将两个io口设置为0；
	DIGITAL_OUT0 = 0;
	DIGITAL_OUT1 = 0;
	//初始化第二层辊轮，主要是将两个io口设置为0；
	DIGITAL_OUT2 = 0;
	DIGITAL_OUT3 = 0;
	eu100_output(0x03, 0x00);
	eu100_output(0x0c, 0x00);
	return 0;
}

int32_t double_roller_huaw_test_init(void) 
{
	int32_t ret = -1;
	
	ret = eu100_output(0xff, 0x00);
	ret = eu100_output(0xff, 0x00);
	return ret;
}

int32_t jql_fork_lift_init(void)
{
	int32_t ret = -1;
	
	/* 顶升电机 */
	ret = kinco_err_clr(JUNGLEACTION_A); delay_ms(100);
	ret = kinco_mode_select(JUNGLEACTION_A);  delay_ms(100);
	ret = kinco_pdo_enable(0x05); delay_ms(100);
	ret = kinco_enable(JUNGLEACTION_A); delay_ms(100);
	ret = kinco_spd_crl(JUNGLEACTION_A,0); delay_ms(100);
	
	ret = jql_fork_encoder_init();
	return ret;
}


/*
 * 板载外围杂类设备初始化
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
int32_t misc_dev_init(void)
{
#ifdef SR_MOTOR_ENABLE
	sr_motor_driver_init();
	tiean_motor_init();
	
	if (stCtlPara.emBaseType == DIFF && stCtlPara.ActionType == AEROPOD) {
		aeropod_climb_motor_init();
	}
	else if (stCtlPara.emBaseType == DIFF && stCtlPara.ActionType == JACKINGROTATE) {
		oasis300_jacking_motor_init();
	}
	else if (stCtlPara.emBaseType == DIFF && stCtlPara.ActionType == HUAWEI_GETTRAY){
		oasis_tray_motor_init();
	} else {
	
	}
#endif

#ifdef ULTRASONIC_ENABLE
		ultraSonic_Init(0XFF);
#endif

	if (stCtlPara.emBaseType == Gulf || stCtlPara.emBaseType == Gulf_14) {
		s300_area_init();
		sr_l14_init();
		safty_relay_reset(); //Gulf safty relay init, DIGITAL_OUT7 cannot be used by other app
		sr_rs485_init(115200);
	} 
	else {
		if (stCtlPara.ActionType == FORK_LIFT) {
			sr_rs485_init(9600);
		} 
	}

	if (stCtlPara.emBaseType != Gulf && stCtlPara.emBaseType != Gulf_14) {
		if (stCtlPara.DownPGV == 1) {
			sr_pgv_init(SR_PGV_NO1);
		}
		else if (stCtlPara.DownPGV == 2 || stCtlPara.DownPGV == 3) {
			pgv_down_stop();
			delay_ms(300);
			pgv_down_init();
		}

		if (stCtlPara.UpperPGV == 1){
			delay_ms(100);
			sr_pgv_init(SR_PGV_NO2);
		}
		else if (stCtlPara.UpperPGV == 2 || stCtlPara.UpperPGV == 3){
			delay_ms(100);
			pgv_upper_stop();
			delay_ms(300);
			pgv_upper_init();
		}
	}

	if (stCtlPara.emBaseType == DIFF && stCtlPara.ActionType == JACKING){
		jacking_down_cmd();
	}
	else if (stNav.emBaseType == DIFF && stCtlPara.ActionType == DOUBLEROLLER){
		double_roller_init();
	}
	else if (stNav.emBaseType == DIFF && stCtlPara.ActionType == DOUBLEROLLER_HUAWEICESHI){
		double_roller_huaw_test_init();
	}
	else if (stNav.emBaseType == DIFF && stCtlPara.ActionType == FORK_LIFT){
		jql_fork_lift_init();
	}

	return 0;
}
