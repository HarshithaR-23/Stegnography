#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"decode.h"
#include<unistd.h>


int main(int argc,char **argv)
{
    if(argc<4)
    {
        if( argc==1)
        {
            printf("INFO : Please pass valid arguements.\n");
            printf("INFO : Encoding - minimum 4 arguments.\n");
            printf("INFO : Deooding - minimum 3 arguements.\n");
            return 0;

        }

        else if(argc==2)
        {
            if(!strcmp(argv[1],"-e"))
            {
                    printf("INFO :  For encoding please pass minimum 4 arguments like--> ./a.out -e source_image secret_data_file.\n ");
                    return 0;
            }
            else if(!strcmp(argv[1],"-d"))
            {
                    printf("INFO : For decoding please pass minimum 3 arguments like--> ./a.out -d  source_image.\n");
                    return 0;
            }
        }
        else if(argc==3)
        {
            if(!strcmp(argv[1],"-e"))
            {
                    printf("INFO : For encoding please pass minimum 4 arguements like--> .a.out -e source_image secret_data_file.\n");
                    return 0;
            }
        }
    }


//checking operation

    if(check_operation_type(argv)== e_encode)
    {
        printf("INFO :you selected encoding ,Encoding started\n");
        sleep(1.5);
        printf("\n");

        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf("INFO : Read and validate completed \n");
            sleep(1.5);
            printf("\n");
            if(do_encoding(&encInfo)==e_success)
            {
                printf("\n");
                printf("Encoding Successfull\n");
                printf("\n");
                printf("\n");
                return 0;
            }
            else
            {
                printf("\n");
                printf("     Encoding Failed\n");
                printf("\n");


            }
        }
        else
        {
            printf("ERROR : Read and Validation Failed\n");
        }
    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("INFO :you selected decoding,Decoding started\n");
        printf("\n");
        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            printf("INFO : Read and validate compelted\n");
            printf("\n");
            if(do_decoding(&decInfo) == e_success)
            {
                printf("\n");
                printf("Decoding Successfull\n");
                printf("\n");
                return 0;
            }
            else
            {
                printf("\n");
                printf("Decoding Failed\n");
                printf("\n");
            }
        }
        else
        {
            printf("ERROR : Read and Validation Failed\n");
        }

    }
    else if(check_operation_type(argv) == e_unsupported)
    {
        printf("ERROR :Please enter valid option : '-e'-->for encoding or '-d'-->for decoding\n");
    }



}
OperationType check_operation_type(char **argv)
{
		if((strcmp(argv[1],"-e"))==0)
		{
				return e_encode;

		}
		else if((strcmp(argv[1],"-d"))==0)
		{
				return e_decode;
		}
		else
		{
				return e_unsupported;
		}



}

