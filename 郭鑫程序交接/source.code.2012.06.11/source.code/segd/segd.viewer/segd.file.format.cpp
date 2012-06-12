#include "../../common/config.h"
#include "../../common/utility.h"
#include "resource.h"
#include "segd.file.format.h"

#if defined(__SEG_D_FILE_FORMAT_428XL_COMPATIBLE_MODE__)
#	include "segd.file.format.428XL.compatible.h"
#elif defined(__SEG_D_FILE_FORMAT_REV_2_1_MODE__)
#	include "segd.file.format.Re2.1.h"
#endif //defined(__SEG_D_FILE_FORMAT_428XL_COMPATIBLE_MODE__)

bool segd_file_create_private(__in_out__ SEGD_FILE_PTR segd_file_ptr)
{	
	LARGE_INTEGER	distance_to_move = {0x0};

	switch(segd_file_ptr->file_create_type_){
		case SEGD_FILE_NULL:
		case SEGD_FILE_CREATE:
			segd_file_ptr->access_mode_	= GENERIC_WRITE;
			segd_file_ptr->share_mode_ = FILE_SHARE_WRITE;		
			segd_file_ptr->creation_disposition_ = CREATE_ALWAYS;
			segd_file_ptr->file_flag_ = FILE_ATTRIBUTE_NORMAL;			
			break;
		case SEGD_FILE_OPEN_EXISTING:
			segd_file_ptr->access_mode_	= GENERIC_READ;
			segd_file_ptr->share_mode_ = FILE_SHARE_READ;		
			segd_file_ptr->creation_disposition_ = OPEN_EXISTING;
			segd_file_ptr->file_flag_ =  0x0;			
			break;
		default:
			SetLastError(IDS_STRING120);
			return false;
	}

	if(segd_file_ptr->file_io_type_ == SEGD_FILE_IO_OVERLAPPED){ segd_file_ptr->file_flag_ |= FILE_FLAG_OVERLAPPED; }
	else if(segd_file_ptr->file_io_type_ == SEGD_FILE_IO_OVERLAPPED_NO_BUFFERING){ segd_file_ptr->file_flag_ |= (FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING); }

	segd_file_ptr->file_handle_ = CreateFile(segd_file_ptr->path_name_, segd_file_ptr->access_mode_, segd_file_ptr->share_mode_,
											NULL, segd_file_ptr->creation_disposition_, segd_file_ptr->file_flag_, NULL);

	if(segd_file_ptr->file_handle_ == INVALID_HANDLE_VALUE){ return false; }

	SetFilePointerEx(segd_file_ptr->file_handle_, distance_to_move, NULL, FILE_BEGIN);
	segd_file_ptr->overlapped_.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	return true;
}

SEGD_FILE_PTR segd_file_create(__in__ LPTSTR path_name, __in__ unsigned int create_type)
{
	SEGD_FILE_PTR segd_file = NULL;

	segd_file = (SEGD_FILE_PTR)malloc(sizeof(SEGD_FILE));
	if(segd_file == NULL){	SetLastError(IDS_STRING128);	return NULL;}

	__matrix_zero_Memory(segd_file, sizeof(*segd_file));
	segd_file->file_io_type_ = create_type & SEGD_FILE_IO_MASK;
	segd_file->file_create_type_ = create_type & SEGD_FILE_CREATE_MASK;
	if(segd_file->file_io_type_ == 0x0) segd_file->file_io_type_ = SEGD_FILE_IO_NON_OVERLAPPED;
	memcpy(segd_file->path_name_, path_name, sizeof(TCHAR) * _tcsclen(path_name));

	switch(segd_file->file_io_type_){
		case SEGD_FILE_IO_NON_OVERLAPPED: 
		case SEGD_FILE_IO_OVERLAPPED:
		case SEGD_FILE_IO_OVERLAPPED_NO_BUFFERING: 
			if(!segd_file_create_private(segd_file)){	free(segd_file); return NULL;}
			break;
		default: 
			SetLastError(IDS_STRING128);
			return NULL;
	}

	return segd_file;
}

void segd_file_close(SEGD_FILE_PTR segd_file)
{
	int i = 0;
	if(segd_file != NULL){

		segd_file_close_private(segd_file); 

		if(segd_file->scan_type_header_ != NULL){

			free(segd_file->scan_type_header_); 
			segd_file->scan_type_header_ = NULL;
		}

		if(segd_file->external_header_.series_ != NULL){ 
			free(segd_file->external_header_.series_); 
			segd_file->external_header_.series_ = NULL;
		}

		if(segd_file->trace_headers_ != NULL){
			for(i = 0; i < segd_file->extended_header_.trace_number_; ++i){
				if(segd_file->trace_headers_[i].trace_data_){
					free(segd_file->trace_headers_[i].trace_data_);
					segd_file->trace_headers_[i].trace_data_ = NULL;
				}
			}
			
			free(segd_file->trace_headers_);
			segd_file->trace_headers_ = NULL;
		}

		if(segd_file->aux_trace_headers_ != NULL){

			for(i = 0; i < segd_file->extended_header_.auxiliary_trace_number_; ++i){
				if(segd_file->aux_trace_headers_[i].trace_data_ != NULL){
					free(segd_file->aux_trace_headers_[i].trace_data_);
					segd_file->aux_trace_headers_[i].trace_data_ = NULL;	
				}
			}

			free(segd_file->aux_trace_headers_);
			segd_file->aux_trace_headers_ = NULL;
		}

		__matrix_zero_Memory(segd_file, sizeof(*segd_file));
		free(segd_file);
		segd_file = NULL;
	}
}

void segd_file_close_private(__in_out__ SEGD_FILE_PTR segd_file_ptr)
{
	if(segd_file_ptr->file_handle_ != NULL){
		CloseHandle(segd_file_ptr->file_handle_);
		segd_file_ptr->file_handle_ = NULL;
	}

	if(segd_file_ptr->overlapped_.hEvent != NULL){
		CloseHandle(segd_file_ptr->overlapped_.hEvent);
		segd_file_ptr->overlapped_.hEvent = NULL;
	}
}

int segd_file_read_non_overlapped_private(__in__ HANDLE file_handler, __in_out__ SEGD_UCHAR* buff, __in__ SEGD_UINT buff_size, __in__ OVERLAPPED* overlapped)
{
	int i_ret = -1;
	LARGE_INTEGER	file_size = {0};
	UINT err = 0x0;
	DWORD number_bytes_read = 0x0;
	BOOL is_ok = TRUE;

	if(file_handler == NULL){	SetLastError(IDS_STRING121); return i_ret = -1; }

	if(GetFileSizeEx(file_handler, &file_size) == 0) return i_ret = 0;
	__matrix_zero_Memory(buff, buff_size * sizeof(SEGD_UCHAR));

	is_ok = ReadFile(file_handler, buff, buff_size, &number_bytes_read, overlapped);
	if(!is_ok){	err = GetLastError();	SetLastError(IDS_STRING122); i_ret = -1; goto END; }

	if(number_bytes_read != buff_size)	{	i_ret = -1;	SetLastError(IDS_STRING122); goto END;}
	else								{	i_ret = number_bytes_read;}

END:
	return i_ret;
}

int segd_file_read_overlapped_private(__in__ HANDLE file_handler, __in_out__ SEGD_UCHAR* buff, __in__ SEGD_UINT buff_size, __in__ OVERLAPPED* overlapped)
{
	int i_ret = -1;
	LARGE_INTEGER	file_size = {0};
	UINT err = 0x0;
	DWORD number_bytes_read = 0x0;
	BOOL is_ok = TRUE;

	if(file_handler == NULL || overlapped == NULL || overlapped->hEvent == NULL){

		SetLastError(IDS_STRING121);
		return i_ret = -1;
	}

	if(GetFileSizeEx(file_handler, &file_size) == 0)	return i_ret = 0;

	__matrix_zero_Memory(buff, buff_size * sizeof(SEGD_UCHAR));
	is_ok = ReadFile(file_handler, buff, buff_size, NULL, overlapped);

	if(!is_ok){

		err = GetLastError();
		if(err == ERROR_IO_PENDING){

			is_ok = GetOverlappedResult(file_handler, overlapped, &number_bytes_read, TRUE);
			if(!is_ok){

				SetLastError(IDS_STRING123);
				i_ret = -1;
				goto END;
			}
		}
		else if(err == ERROR_HANDLE_EOF){

			SetLastError(IDS_STRING122);
			i_ret = -1;
			goto END;
		}
	}

	if(number_bytes_read != buff_size){

		i_ret = -1;
		SetLastError(IDS_STRING122);
		goto END;
	}
	else{

		i_ret = number_bytes_read;
	}

END:
	return i_ret;
}

int segd_file_read_private(__in__ SEGD_FILE_PTR file_handle, __in_out__ SEGD_UCHAR* buff, __in__ SEGD_UINT buff_size)
{
	int i_ret = 0x0;

	file_handle->overlapped_.Offset = file_handle->file_pointer_.LowPart;
	file_handle->overlapped_.OffsetHigh = file_handle->file_pointer_.HighPart;
	switch(file_handle->file_io_type_){
		case SEGD_FILE_IO_OVERLAPPED:
			i_ret = segd_file_read_overlapped_private(file_handle->file_handle_, buff, buff_size, &file_handle->overlapped_);
			if(i_ret == buff_size) segd_file_seek_private(file_handle->file_handle_, buff_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
			break;
		case SEGD_FILE_IO_NON_OVERLAPPED:
			i_ret = segd_file_read_non_overlapped_private(file_handle->file_handle_, buff, buff_size, &file_handle->overlapped_);
			break;
		case SEGD_FILE_IO_OVERLAPPED_NO_BUFFERING:
			break;
	}

	return i_ret;
}

int segd_file_write_overlapped_private(__in__ HANDLE file_handler, __in__ SEGD_UCHAR* buff, __in__ SEGD_UINT buff_size, __in__ OVERLAPPED* overlapped)
{
	int i_ret = -1;
	UINT err = 0x0;
	DWORD number_bytes_read = 0x0;
	BOOL is_ok = TRUE;

	if(file_handler == NULL || overlapped == NULL){	SetLastError(IDS_STRING121);return i_ret = -1;}

	is_ok = WriteFile(file_handler, buff, buff_size, NULL, overlapped);
	if(!is_ok){

		err = GetLastError();
		if(err == ERROR_IO_PENDING){

			is_ok = GetOverlappedResult(file_handler, overlapped, &number_bytes_read, TRUE);
			if(!is_ok){

				SetLastError(IDS_STRING123);
				i_ret = -1;
				goto END;
			}
		}
		else if(err == ERROR_HANDLE_EOF){

			SetLastError(IDS_STRING122);
			i_ret = -1;
			goto END;
		}
	}

	if(number_bytes_read != buff_size){	i_ret = -1;	SetLastError(IDS_STRING122);	goto END;}
	else{	i_ret = number_bytes_read;	}

	FlushFileBuffers(file_handler);
END:
	return i_ret;
}

int segd_file_write_non_overlapped_private(__in__ HANDLE file_handler, __in__ SEGD_UCHAR* buff, __in__ SEGD_UINT buff_size)
{
	int i_ret = -1;
//	UINT err = 0x0;
	DWORD number_bytes_read = 0x0;
	BOOL is_ok = TRUE;

	if(file_handler == NULL || buff == NULL || buff_size == 0){	SetLastError(IDS_STRING121); return i_ret; }

	is_ok = WriteFile(file_handler, buff, buff_size, &number_bytes_read, NULL);
	if(!is_ok){	SetLastError(IDS_STRING123); return i_ret; }

	if(number_bytes_read != buff_size)	{ SetLastError(IDS_STRING122); return i_ret; }
	else								{	i_ret = number_bytes_read; }

	return i_ret;
}

int segd_file_write_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_UCHAR* buff, __in__ SEGD_UINT buff_size)
{
	int i_ret = 0x0;

	switch(file_handle->file_io_type_){
		case SEGD_FILE_IO_OVERLAPPED:
			file_handle->overlapped_.Offset = file_handle->file_pointer_.LowPart;
			file_handle->overlapped_.OffsetHigh = file_handle->file_pointer_.HighPart;
			i_ret = segd_file_write_overlapped_private(file_handle->file_handle_, buff, buff_size, &file_handle->overlapped_);
			if(i_ret == buff_size) segd_file_seek_private(file_handle->file_handle_, buff_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
			break;
		case SEGD_FILE_IO_NON_OVERLAPPED:
			i_ret = segd_file_write_non_overlapped_private(file_handle->file_handle_, buff, buff_size);
			break;
		case SEGD_FILE_IO_OVERLAPPED_NO_BUFFERING:
			break;
	}

	return i_ret;
}

bool segd_file_seek_private(__in__ HANDLE file_handle, __in__ LONGLONG distance_to_move, 
					__in__ enum __enuSEGD_FILE_MOVE_METHOD move_method, __in_out__ LONGLONG* file_pointer)
{
	LARGE_INTEGER li;
	LARGE_INTEGER li2 = {0};
	DWORD method = 0;

	switch(move_method){
		case SEGD_FILE_BEGIN:	method = FILE_BEGIN; break;
		case SEGD_FILE_CURRENT:	method = FILE_CURRENT;break;
		case SEGD_FILE_END:	method = FILE_END;break;
	}

	li.QuadPart = distance_to_move;
	if(!SetFilePointerEx(file_handle, li, &li2, method))	return false;
	if(file_pointer != NULL)	*file_pointer = li2.QuadPart;
	
	return true;
};

bool segd_file_seek_begin_private(__in__ HANDLE file_handle, __in_out__ LONGLONG* file_pointer)
{
	bool is_ok = true;
	if(segd_file_seek_private(file_handle, 0, SEGD_FILE_BEGIN, file_pointer) != 0) is_ok = false;
	return is_ok;
}

bool segd_file_seek_end_private(__in__ HANDLE file_handle, __in_out__ LONGLONG* file_pointer)
{
	bool is_ok = true;
	if(segd_file_seek_private(file_handle, 0, SEGD_FILE_END, file_pointer) != 0) is_ok = false;
	return is_ok;
}

