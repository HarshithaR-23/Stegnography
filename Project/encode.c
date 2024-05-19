#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<string.h>
#include<unistd.h>

/* Function Definitions */
Status read_and_validate_encode_args(char **argv, EncodeInfo *encInfo)
{

		if(!strcmp((strstr(argv[2],".")),".bmp"))
		{
				encInfo->src_image_fname=argv[2];
		}
		else
		{
				return e_failure;
		}
		if((strstr(argv[3],".txt")!=NULL))
		{
				encInfo->secret_fname=argv[3];
			    strcpy(encInfo->extn_secret_file,".txt");
		}
		else if((strstr(argv[3],".sh")!=NULL))
		{
				encInfo->secret_fname=argv[3];
				strcpy(encInfo->extn_secret_file,".sh");
		}
		else if((strstr(argv[3],".c")!=NULL))
		{
				encInfo->secret_fname=argv[3];
				strcpy(encInfo->extn_secret_file,".c");
		}
		else if((strstr(argv[3],".pdf")!=NULL))
		{
				encInfo->secret_fname=argv[3];
				strcpy(encInfo->extn_secret_file,".pdf");
		}
		else
		{
				return e_failure;
		}
		if(argv[4])
		{
				encInfo->stego_image_fname=argv[4];
				printf("INFO :Output  file is mentioned, It is added\n");
				sleep(1.5);
				printf("\n");
		}
		else
		{
				encInfo->stego_image_fname="encode.bmp";
				printf("INFO : Output file not mentioned,Default file name is added\n");
			    sleep(1.5);
				printf("\n");
		}
		return e_success;
}
//---------------------------------------------------------------------------------------
Status do_encoding(EncodeInfo *encInfo)
{
		//Step1:Call open_files
		//Step2:check If success ->goto step3
		//else _>print error,return failure.
		//step3:Call check_capacity()
		//step4:if success->goto step5
		//else->print error,return failure
		//step5:call copy_bmp_header():
		//step6:if success->goto step 7
		//else->print error,return failure.
		//step7:call encode_magic_string()
		//step8:if success-> goto step9
		//else ->print error retun_falure 
		if(open_files(encInfo)==e_success)
		{
				printf("INFO : Open files executed successfully.\n");
				sleep(1.5);
				printf("\n");
				

		}
		else
		{
				printf("ERROR : Open file function not executed successfully.\n");
				return e_failure;
		}
		if(check_capacity(encInfo)==e_success)
		{
				//printf("place in check capacity = %ld\n", ftell(encInfo -> fptr_src_image));
				printf("INFO : check capacity is done, we have  enough space to encode.\n");
				printf("\n");
		}
		else
		{
				printf("ERROR : NO space in image file.\n ");
				return e_failure;
		}
		if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
		{
				//printf("place in copy bmp header = %ld\n", ftell(encInfo -> fptr_src_image));
				printf("INFO : Header of 'bmp' copied to stego. \n");
				printf("\n");
		}
		else
		{
				printf("ERROR :Failued to copy header of 'bmp'. \n");
				return e_failure;
		}
		if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
		{
				//printf("place in magic string = %ld\n", ftell(encInfo -> fptr_src_image));
				printf("INFO : 'MAGIC_STRING' was suceessfully encoded.	\n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Failed to encode 'MAGIC_STRING'.\n ");
				return e_failure;

		}
		//step9:call encode_secret_file_extn_size()
		//step 10: if success -> goto step11
		//else retunr failure
		if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success)
		{
				printf("INFO : Successful in encoding secert file extension size.  \n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Secret file extension size was not successfull.\n ");
				return e_failure;
		}
		if(encode_secret_file_extn(encInfo -> extn_secret_file,encInfo)==e_success)
		{
				printf("INFO : Successful in encoding  secret file extension.\n");
				printf("\n");

		}
		else
		{
				printf("ERROR :  Failed in encoding secret file extension.\n");
				return e_failure; 

		}
        if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
		{
				printf("INFO : Successful in encoding secret file size. \n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Failed in encoding secret file size. \n");
				return e_failure;
		}
		if(encode_secret_file_data(encInfo)==e_success)
		{
				printf("INFO : Successful in encoding secret data.\n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Failed in encoding secret data.\n");
				return e_failure;
		}
		if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
		{
				printf("INFO : Successful in copying remaining data.\n");
				printf("\n");
		}
		else
		{
				printf("ERROR : Failed  in  copying remaining data.\n");
				return e_failure;
		}

//step 11:call encode _secret_file_extn()
		//step 12: if success->go step>goto step13
		//else print error
		return e_success;
}
//----------------------------------------------------------------------------------------------------------------------------------
Status check_capacity(EncodeInfo *encInfo)
{
		//step 1:  find src_image_fname size
		//step 2: find secret file size->store it in fil size_secret_file
		//step 3: calculate the value of how much byte neededfor encoding
		// magic_string(8*2) + size_of_extn(4*8)+
		//step 4:comapre image_capacity>calculation
		//step 5: if true retun e_success
		
		//else retun  e_failure
         //step1:
		//rewind(encInfo -> fptr_src_image);
        encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
        //printf("%d\n",encInfo->image_capacity);
		//step2:
		fseek(encInfo->fptr_secret,0,SEEK_END);
		encInfo->size_secret_file=ftell(encInfo->fptr_secret);
		//printf("Secret file size is %ld",encInfo->size_secret_file);
		int calculate = (strlen(MAGIC_STRING)*8) + 32 +32 + 32+ (sizeof(encInfo->size_secret_file)*8);
		//printf("%d\n",calculate);
		if(encInfo->image_capacity>calculate)
		{
			return e_success;
		}
		else
		{
				return e_failure;
		}
}
//-------------------------------------------------------------------------------------------------------------------------------------
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)
{
		rewind(fptr_src_image);
		//printf("place 1 = %ld\n", ftell(fptr_src_image));
		char buffer[55];
		fread(buffer,54,1,fptr_src_image);
		fwrite(buffer,54,1,fptr_stego_image);
		//printf("place 2 = %ld\n", ftell(fptr_src_image));
		return e_success;

}
//--------------------------------------------------------------------------------------------------------------------------------------
Status encode_magic_string(char *magic_string,EncodeInfo *encInfo)
{
		///printf("Magic string = %s\n", magic_string);
		//printf("place = %ld\n", ftell(encInfo -> fptr_src_image));
	if	(encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		//printf("place = %ld\n", ftell(encInfo -> fptr_src_image));
			return e_success;

	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
Status encode_data_to_image(char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
		char image_buffer[8];
		//step1:read 8 bytes from source image
		//step 2:call encode_byte_to_lsb();
		//encode_byte_to_lsb(data[i],image_buffer);
		//step3: write 8bytesfro image_ buffer to stego_image;
		//step4::repeat  this  size time
		for(int i=0;i<size;i++)
		{
				fread(image_buffer,8,1,fptr_src_image);
				encode_byte_to_lsb(data[i],image_buffer);
				fwrite(image_buffer,8,1,fptr_stego_image);
				
		}
		return e_success;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
Status encode_byte_to_lsb(char ch,char *image_buffer)
{
		//step1 : get 1 bit  from ch
		//step2 :clear LSB qfrom image_buffer[i]
		//step3 : Replace
		//step4 :repeat this procces for 8 times
		for(int i=0;i<8;i++)
		{
			char get=ch&(1<<i);
			get=((unsigned)get>>i);
			image_buffer[i]=(image_buffer[i]&(~(1)))|get;			

		}
		return e_success;

}
//--------------------------------------------------------------------------------------------------------------------------------------------
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
		//32 bytes from source image
		//step1
		char ch;
		char data_buffer[1];
		for(int i=0;i<32;i++)		
		{
				ch = ((size & (1 << i)) >> i);
			 fread(data_buffer,1,1,encInfo->fptr_src_image);
			 data_buffer[0] = (data_buffer[0] & (~(1)) | ch);
			fwrite(data_buffer,1,1,encInfo->fptr_stego_image);
		}
		return e_success;
}		
//---------------------------------------------------------------------------------------------------------------------------------------------
Status encode_secret_file_extn(char *secret_file_extn,EncodeInfo	*encInfo)
{
		encode_data_to_image(secret_file_extn,strlen(secret_file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
		return e_success;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
Status  encode_secret_file_size(long size,EncodeInfo	*encInfo)
{
encode_secret_file_extn_size(size,encInfo);
return e_success;
}
///
Status encode_secret_file_data(EncodeInfo *encInfo)
{
fseek(encInfo->fptr_secret,0,SEEK_SET);
char secret_buffer[encInfo->size_secret_file];
fread(secret_buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);
encode_data_to_image(secret_buffer,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
return e_success;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
Status copy_remaining_img_data(FILE *fptr_src_image,FILE *fptr_stego_image)
{
char ch;
while((fread(&ch,1,1,fptr_src_image))>0)
{
fwrite(&ch,1,1,fptr_stego_image);
}
return e_success;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
//-----------------------------------------------------------------------------------------------------------------------------------------------
uint get_image_size_for_bmp(FILE *fptr_image)
{
		uint width, height;
		// Seek to 18th byte
		fseek(fptr_image, 18, SEEK_SET);

		// Read the width (an int)
		fread(&width, sizeof(int), 1, fptr_image);
		printf("IMAGE width = %u\n", width);

		// Read the height (an int)
		fread(&height, sizeof(int), 1, fptr_image);
		printf("IMAGE height = %u\n", height);

		// Return image capacity
		return width * height * 3;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
//--------------------------------------------------------------------------------------------------------------------------------------------------
Status open_files(EncodeInfo *encInfo)
{
		// Src Image file
		encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
		// Do Error handling
		if (encInfo->fptr_src_image == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

				return e_failure;
		}

		// Secret file
		encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
		// Do Error handling
		if (encInfo->fptr_secret == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

				return e_failure;
		}

		// Stego Image file
		encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
		// Do Error handling
		if (encInfo->fptr_stego_image == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

				return e_failure;
		}

		// No failure return e_success
		return e_success;
}
