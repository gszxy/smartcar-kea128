/*****************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file kexx_config.h
*
* @author Freescale
*
* @version 0.0.1
*
* @date Oct 15, 2013
*
* @brief library and board configuration code. 
*
*******************************************************************************
*
* NOTE:
*	printf call may occupy a lot of memory (around 1924 bytes), so please
*	consider your code size before using printf.
******************************************************************************/

#ifndef _KExx_CONFIG_H_
#define _KExx_CONFIG_H_

/******************************************************************************
* Includes
******************************************************************************/
#include <stdint.h>
/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros
******************************************************************************/

//#define     TEST_ON_EVB           /*!< if running on freedom board, this line must be commented */
#define TRIM_IRC                    /*!< trim internal reference clock */

//#define SPI0_PINREMAP             /*!< remap SPI0 pins to alternative pins */
#define DISABLE_NMI                 /*!< disable NMI function */

/* HW abstraction for register name remap */
#define FTMRH           FTMRE

/* FCLKDIV Bit Fields */                                                                                  
#define FTMRH_FCLKDIV_FDIV_MASK         FTMRE_FCLKDIV_FDIV_MASK                                           
#define FTMRH_FCLKDIV_FDIV_SHIFT        FTMRE_FCLKDIV_FDIV_SHIFT                                          
#define FTMRH_FCLKDIV_FDIV(x)           FTMRE_FCLKDIV_FDIV(x)                                             
#define FTMRH_FCLKDIV_FDIVLCK_MASK      FTMRE_FCLKDIV_FDIVLCK_MASK                                        
#define FTMRH_FCLKDIV_FDIVLCK_SHIFT     FTMRE_FCLKDIV_FDIVLCK_SHIFT                                       
#define FTMRH_FCLKDIV_FDIVLD_MASK       FTMRE_FCLKDIV_FDIVLD_MASK                                         
#define FTMRH_FCLKDIV_FDIVLD_SHIFT      FTMRE_FCLKDIV_FDIVLD_SHIFT                                        
/* FSEC Bit Fields */                                                                         
#define FTMRH_FSEC_SEC_MASK             FTMRE_FSEC_SEC_MASK                                               
#define FTMRH_FSEC_SEC_SHIFT            FTMRE_FSEC_SEC_SHIFT                                              
#define FTMRH_FSEC_SEC(x)               FTMRE_FSEC_SEC(x)                                                 
#define FTMRH_FSEC_KEYEN_MASK           FTMRE_FSEC_KEYEN_MASK                                             
#define FTMRH_FSEC_KEYEN_SHIFT          FTMRE_FSEC_KEYEN_SHIFT                                            
#define FTMRH_FSEC_KEYEN(x)             FTMRE_FSEC_KEYEN(x)                                               
/* FCCOBIX Bit Fields */                                                                         
#define FTMRH_FCCOBIX_CCOBIX_MASK       FTMRE_FCCOBIX_CCOBIX_MASK                                         
#define FTMRH_FCCOBIX_CCOBIX_SHIFT      FTMRE_FCCOBIX_CCOBIX_SHIFT                                        
#define FTMRH_FCCOBIX_CCOBIX(x)         FTMRE_FCCOBIX_CCOBIX(x)                                           
/* FCNFG Bit Fields */                                                                      
#define FTMRH_FCNFG_FSFD_MASK           FTMRE_FCNFG_FSFD_MASK                                             
#define FTMRH_FCNFG_FSFD_SHIFT          FTMRE_FCNFG_FSFD_SHIFT                                            
#define FTMRH_FCNFG_FDFD_MASK           FTMRE_FCNFG_FDFD_MASK                                             
#define FTMRH_FCNFG_FDFD_SHIFT          FTMRE_FCNFG_FDFD_SHIFT                                            
#define FTMRH_FCNFG_IGNSF_MASK          FTMRE_FCNFG_IGNSF_MASK                                            
#define FTMRH_FCNFG_IGNSF_SHIFT         FTMRE_FCNFG_IGNSF_SHIFT                                           
#define FTMRH_FCNFG_CCIE_MASK           FTMRE_FCNFG_CCIE_MASK                                             
#define FTMRH_FCNFG_CCIE_SHIFT          FTMRE_FCNFG_CCIE_SHIFT                                            
/* FERCNFG Bit Fields */                                                                    
#define FTMRH_FERCNFG_SFDIE_MASK        FTMRE_FERCNFG_SFDIE_MASK                                          
#define FTMRH_FERCNFG_SFDIE_SHIFT       FTMRE_FERCNFG_SFDIE_SHIFT                                         
#define FTMRH_FERCNFG_DFDIE_MASK        FTMRE_FERCNFG_DFDIE_MASK                                          
#define FTMRH_FERCNFG_DFDIE_SHIFT       FTMRE_FERCNFG_DFDIE_SHIFT                                         
/* FSTAT Bit Fields */                                                                        
#define FTMRH_FSTAT_MGSTAT_MASK         FTMRE_FSTAT_MGSTAT_MASK                                           
#define FTMRH_FSTAT_MGSTAT_SHIFT        FTMRE_FSTAT_MGSTAT_SHIFT                                          
#define FTMRH_FSTAT_MGSTAT(x)           FTMRE_FSTAT_MGSTAT(x)                                             
#define FTMRH_FSTAT_MGBUSY_MASK         FTMRE_FSTAT_MGBUSY_MASK                                           
#define FTMRH_FSTAT_MGBUSY_SHIFT        FTMRE_FSTAT_MGBUSY_SHIFT                                          
#define FTMRH_FSTAT_FPVIOL_MASK         FTMRE_FSTAT_FPVIOL_MASK                                           
#define FTMRH_FSTAT_FPVIOL_SHIFT        FTMRE_FSTAT_FPVIOL_SHIFT                                          
#define FTMRH_FSTAT_ACCERR_MASK         FTMRE_FSTAT_ACCERR_MASK                                           
#define FTMRH_FSTAT_ACCERR_SHIFT        FTMRE_FSTAT_ACCERR_SHIFT                                          
#define FTMRH_FSTAT_CCIF_MASK           FTMRE_FSTAT_CCIF_MASK                                             
#define FTMRH_FSTAT_CCIF_SHIFT          FTMRE_FSTAT_CCIF_SHIFT                                            
/* FERSTAT Bit Fields */                                                                     
#define FTMRH_FERSTAT_SFDIF_MASK        FTMRE_FERSTAT_SFDIF_MASK                                          
#define FTMRH_FERSTAT_SFDIF_SHIFT       FTMRE_FERSTAT_SFDIF_SHIFT                                         
#define FTMRH_FERSTAT_DFDIF_MASK        FTMRE_FERSTAT_DFDIF_MASK                                          
#define FTMRH_FERSTAT_DFDIF_SHIFT       FTMRE_FERSTAT_DFDIF_SHIFT                                         
/* FPROT Bit Fields */                                                                   
#define FTMRH_FPROT_FPLS_MASK           FTMRE_FPROT_FPLS_MASK                                             
#define FTMRH_FPROT_FPLS_SHIFT          FTMRE_FPROT_FPLS_SHIFT                                            
#define FTMRH_FPROT_FPLS(x)             FTMRE_FPROT_FPLS(x)                                               
#define FTMRH_FPROT_FPLDIS_MASK         FTMRE_FPROT_FPLDIS_MASK                                           
#define FTMRH_FPROT_FPLDIS_SHIFT        FTMRE_FPROT_FPLDIS_SHIFT                                          
#define FTMRH_FPROT_FPHS_MASK           FTMRE_FPROT_FPHS_MASK                                             
#define FTMRH_FPROT_FPHS_SHIFT          FTMRE_FPROT_FPHS_SHIFT                                            
#define FTMRH_FPROT_FPHS(x)             FTMRE_FPROT_FPHS(x)                                               
#define FTMRH_FPROT_FPHDIS_MASK         FTMRE_FPROT_FPHDIS_MASK                                           
#define FTMRH_FPROT_FPHDIS_SHIFT        FTMRE_FPROT_FPHDIS_SHIFT                                          
#define FTMRH_FPROT_FPOPEN_MASK         FTMRE_FPROT_FPOPEN_MASK                                           
#define FTMRH_FPROT_FPOPEN_SHIFT        FTMRE_FPROT_FPOPEN_SHIFT                                          
/* EEPROT Bit Fields */                                                                    
#define FTMRH_EEPROT_DPS_MASK           FTMRE_EEPROT_DPS_MASK                                             
#define FTMRH_EEPROT_DPS_SHIFT          FTMRE_EEPROT_DPS_SHIFT                                            
#define FTMRH_EEPROT_DPS(x)             FTMRE_EEPROT_DPS(x)                                               
#define FTMRH_EEPROT_DPOPEN_MASK        FTMRE_EEPROT_DPOPEN_MASK                                          
#define FTMRH_EEPROT_DPOPEN_SHIFT       FTMRE_EEPROT_DPOPEN_SHIFT                                         
/* FCCOBHI Bit Fields */                                                                        
#define FTMRH_FCCOBHI_CCOB_MASK         FTMRE_FCCOBHI_CCOB_MASK                                           
#define FTMRH_FCCOBHI_CCOB_SHIFT        FTMRE_FCCOBHI_CCOB_SHIFT                                          
#define FTMRH_FCCOBHI_CCOB(x)           FTMRE_FCCOBHI_CCOB(x)                                             
/* FCCOBLO Bit Fields */                                                                      
#define FTMRH_FCCOBLO_CCOB_MASK         FTMRE_FCCOBLO_CCOB_MASK                                           
#define FTMRH_FCCOBLO_CCOB_SHIFT        FTMRE_FCCOBLO_CCOB_SHIFT                                          
#define FTMRH_FCCOBLO_CCOB(x)           FTMRE_FCCOBLO_CCOB(x)                                             
/* FOPT Bit Fields */                                                                                
#define FTMRH_FOPT_NV_MASK              FTMRE_FOPT_NV_MASK                                                
#define FTMRH_FOPT_NV_SHIFT             FTMRE_FOPT_NV_SHIFT                                               
#define FTMRH_FOPT_NV(x)                FTMRE_FOPT_NV(x)                                                  

         


/*! define print system log */      
/* NOTE: for RAM version, comment it */
//#define PRINT_SYS_LOG               /*!< this macro prints system log information */

#if     !defined(BOOT_LOADER)
    #define PRINT_SYS_LOG           /*!< this macro prints system log information */
#endif

/* define output bus clock */
//#define OUTPUT_BUSCLK             /*!< this macro allows output of bus clock */
#define ICS_TRIM_VALUE  0x58        /*!< trim to max internal clock frequency supported */

#if defined(TEST_ON_EVB)

    /*! define what clock mode at startup on EVB board
     * if USE_FEE is not defined by commenting out the following line, 
     * then it will use FEI mode which is default mode 
     * after reset  
     */
    #define USE_FEE                       /*!< use FEE clock mode */	
    //#define USE_FEE_OSC                     /*!< use FEE clock mode with external active oscillator */
    //#define USE_FEI                       /*!< use FEI clock mode */

    /* define external crystal frequency */
    //#define EXT_CLK_FREQ_KHZ	32          /* in KHz */
    //#define EXT_CLK_FREQ_KHZ	4000        /* in KHz */
    #define EXT_CLK_FREQ_KHZ	8000        /* in KHz */
    //#define EXT_CLK_FREQ_KHZ	10000       /* in KHz */

    //#define CRYST_HIGH_GAIN
    //#define XOSC_STOP_ENABLE

    /* define SCI port # to be used */
    #define TERM_PORT   UART2              /*!< UART2 is used on KEA128 EVB board */

#else
    /*! define what clock mode at startup on freedom board
     * if USE_FEE is not defined by commenting out the following line, 
     * then it will use FEI mode which is default mode 
     * after reset.  
     */
    #define USE_FEE                         /*!< use FEE clock mode */	
    //#define USE_FEE_OSC                   /*!< use FEE clock mode with external active oscillator */
    //#define USE_FEI                       /*!< use FEI clock mode */

    /*! define external crystal frequency. */
    //#define EXT_CLK_FREQ_KHZ	32          /* in KHz */
    //#define EXT_CLK_FREQ_KHZ	4000        /* in KHz */
    //#define EXT_CLK_FREQ_KHZ	8000        /* in KHz */
    #define EXT_CLK_FREQ_KHZ	16000       /* in KHz */

    /*! define UART port # to be used */
    #define TERM_PORT   UART2               /*!< UART2 is used on TRK-KEA128 board */
    /* define SCI port # to be used */
    #define  TERM_PORT              UART2              /*!< UART2 is used on KEA128 EVB board */
    #define  CORE_CLK_HZ            40000000     //系统时钟
    #define  BUS_CLK_HZ             40000000     //bus 时钟
    #define  BUS_CLK_KHZ            40000        //bus 时钟
    #define  BUS_CLK_MHZ            40 
    #define  UART_PRINT_BITRATE     9600 
                      
#endif

    /* define BUS clock in HZ */
//    #if	defined(USE_FEI)
//        #define  BUS_CLK_HZ		24000000L
//    #elif  (EXT_CLK_FREQ_KHZ == 20000)
//        #define  BUS_CLK_HZ		20000000L
//    #elif  (EXT_CLK_FREQ_KHZ == 10000)
//        #define  BUS_CLK_HZ		25000000L
//    #elif  (EXT_CLK_FREQ_KHZ == 8000)
//        #define  BUS_CLK_HZ		20000000L
//    #elif  (EXT_CLK_FREQ_KHZ == 4000)
//        #define  BUS_CLK_HZ		20000000L
//    #elif  (EXT_CLK_FREQ_KHZ == 32)
//        #define  BUS_CLK_HZ		20971520L
//    #else   
//        #define  BUS_CLK_HZ		20000000L
//    #endif

    /*! define UART baud rate */
//    #define UART_PRINT_BITRATE              115200      /*! UART bit rate */

/*! USER LED definitions */
/*! LED maps 
 * LED0  <----> PTC0
 * LED1  <----> PTC1
 * LED2  <----> PTC2
 * LED3  <----> PTC3 
 */
#define LED0_Init()				GPIOA->PDDR |= (1<<16)    /*! RED Light */
#define LED0_Toggle()		    GPIOA->PTOR = (1<<16)
#define LED0_On()		        GPIOA->PSOR = (1<<16)
#define LED0_Off()		        GPIOA->PCOR = (1<<16)

#define LED1_Init()				GPIOA->PDDR |= (1<<17)    /*! Green */
#define LED1_Toggle()		    GPIOA->PTOR = (1<<17)
#define LED1_On()		        GPIOA->PSOR = (1<<17)
#define LED1_Off()		        GPIOA->PCOR = (1<<17)

#define LED2_Init()				GPIOA->PDDR |= (1<<18)  /*! Blue */
#define LED2_Toggle()		    GPIOA->PTOR = (1<<18)
#define LED2_On()		        GPIOA->PSOR = (1<<18)
#define LED2_Off()		        GPIOA->PCOR = (1<<18)

#define LED3_Init()				GPIOA->PDDR |= (1<<19)  /*! Blue */
#define LED3_Toggle()		    GPIOA->PTOR = (1<<19)
#define LED3_On()		        GPIOA->PSOR = (1<<19)
#define LED3_Off()		        GPIOA->PCOR = (1<<19)
/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

#endif /* _KExx_CONFIG_H_ */
