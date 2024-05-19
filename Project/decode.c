#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include<string.h>
#include<unistd.h>

/* ________________________Function Definitions_________________________________*/

Status read_and_validate_decode_args(char **argv, DecodeInfo *decInfo)
{

		if(!strcmp((strstr(argv[2],".")),".bmp"))
		{
				decInfo->src_image_fname=argv[2];
		}
		else
		{
				return e_failure;
		}
		if(argv[3] )
		{
				printf("INFO : Output file mentioned,It is added\n");
				printf("\n");
				strcpy(decInfo->secret_fname_buffer,argv[3]);
		}
		else
		{
				printf("INFO : Output file not mentioned,default name is added\n");
				printf("\n");
				strcpy(decInfo->secret_fname_buffer,"Output");
		}
		return e_success;
}
//--------------------------------------------------------------------------------------------------------------------------------
Status do_decoding(DecodeInfo *decInfo)
{
		if(Open_files(decInfo)==e_success)
		{
				printf("INFO : Open files executed successfully.\n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Open file function not executed successfully.\n");
				return e_failure;
		}
		if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
		{
				printf("INFO : 'MAGIC_STRING' was suceessfully decoded.	\n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Failed to decode 'MAGIC_STRING'.\n ");
				return e_failure;
		}
		if(decode_extn_size(decInfo)==e_success)
		{
				printf("INFO : file extension size decoded successfully.\n");
				printf("\n");
		}
		else
		{
				return e_failure;
		}
		if(decode_secret_file_extn(decInfo)==e_success)
		{
				{
						strcat(decInfo->secret_fname_buffer,decInfo->extn_secret_file);
				}
				printf("INFO : file exention decoded successfully.\n");
				printf("\n");
				// Secret file
				decInfo->fptr_secret = fopen(decInfo->secret_fname_buffer, "w");
				// Do Error handling
				if (decInfo->fptr_secret == NULL)
				{
						perror("fopen");
						fprintf(stderr, "ERROR: Unable to open Output file %s\n", decInfo->secret_fname_buffer);

						return e_failure;
				}
				printf("INFO : Output file opened successfully.\n");
				printf("\n");
		}
		else
		{
				return e_failure;
		}
		if(decode_secret_file_size(decInfo)==e_success)
		{
				printf("INFO : secret file size decoded succesfully\n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Unable deocde  secret file size \n");
				return e_failure;
		}
		if(decode_secret_file_data(decInfo)==e_success)
		{
				printf("INFO : secret file data  decoded successfully\n");
				sleep(1.5);
				printf("\n");
		}
		else
		{
				printf("ERROR : Unable to decode secret file data \n");
				return e_failure;
		}
				return e_success;
}
//--------------------------------------------------------------------------------------------------------------------------------------
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo)
{
		fseek(decInfo->fptr_src_image,54, 0);
		char MagicString_buffer[3]={0};
		for(int i=0;i<2;i++)
		{
				decode_data_from_image(MagicString_buffer,8,i,decInfo->fptr_src_image);
		}
		if(!strcmp(magic_string,MagicString_buffer))
		{
				return e_success;
		}
		else
		{
				return e_failure;
		}
}
//---------------------------------------------------------------------------------------------------------------------------------------
Status decode_data_from_image(char *data,int size,int k,FILE *fptr_src_image)
{
		char ch;
		for(int i=0;i<size;i++)
		{
				fread(&ch,1,1,fptr_src_image);
				ch= ch & 1;
				data[k]=( data[k] | (ch << (i) ) );
		}
		return e_success;
}
//----------------------------------------------------------------------------------------------------------------------------------------
Status decode_extn_size(DecodeInfo *decInfo)
{
		char size_buffer[32],ch;
		int size=0;
		fread(size_buffer,1,32,decInfo->fptr_src_image);
		for(int i=0;i<32;i++)
		{
				size = ( size   ) | (( size_buffer[i] & 1 )<<i);
     	}
		decInfo->extn_size=size;
		return e_success;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
		int i;
		for(int i =0; i< decInfo -> extn_size; i++)
		{
				decInfo -> extn_secret_file[i] = 0;
		}
		for( i=0;i<decInfo->extn_size;i++)
		{
				decode_data_from_image(decInfo->extn_secret_file,8,i,decInfo->fptr_src_image);
		}
		decInfo->extn_secret_file[i]='\0';
		return e_success;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
Status decode_secret_file_size(DecodeInfo *decInfo)
{
		char filesize_buffer[32];
		int size=0;
		fread(filesize_buffer,1,32,decInfo->fptr_src_image);
		for(int i=0; i<32;i++)
		{
				size= (size) | ((filesize_buffer[i] &1)<<i);
		
		}
		decInfo->size_secret_file=size;
		return e_success;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
Status decode_secret_file_data(DecodeInfo *decInfo)
{
		char  data_buffer[8],ch=0;
		fseek(decInfo->fptr_secret,0,SEEK_SET);
		for(int i=0;i<decInfo->size_secret_file;i++)
		{
				fread(data_buffer,8, 1,decInfo->fptr_src_image);
				ch =0;
				for(int j=0;j<8;j++)
				{
						ch= ch | ((data_buffer[j] & 1 )<<j);
				}
				fwrite(&ch,1,1,decInfo->fptr_secret);
		}
		return e_success;
}
//------------------------------------------------------------------------------------------------------------------------------------
/** Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
//---------------------------------------------------------------------------------------------------------------------------------------
Status Open_files(DecodeInfo *decInfo)
{
		// Src Image file
		decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
		// Do Error handling
		if (decInfo->fptr_src_image == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->src_image_fname);

				return e_failure;
		}
		return e_success;
}
