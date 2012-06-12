/**
@brief	实现SEGD文件的读写操作
@details 针对428XL手册中定义的SEGD格式(2.0)的实现
@file	segd.viewer2.h 
@note	程序中部分实现使用了结构体,用块读取方式实现,因此要注意对齐问题
@see	参看428XLUser's Manual Vol2
@version	1.1.23.48.2011.10.27
@author	kuoxin	
@bug
	1.1.22.45.2011.10.08	调整程序,加入对于外部头段和跟踪到4-7道头的读取,修改错误有关读取外部头段
	1.1.23.48.2011.10.27	修改on_open_file函数中,计算读取SEGD数据读取大小方法,当前修改为:
							(道头中的number_of_samples_per_trace * sizeof(SEGD_UCHAR) * 每个数据大小 * 采样频率) / 1000 

*/


void on_open_file(HWND window_handle)
{
	int i = 0;
	ULONGLONG size = 0;
	DWORD err = 0x0;
	bool is_ok = true;

	OPENFILENAME ofn;

	TCHAR szFile[MAX_STRING_SIZE + 4] = {0};
	TCHAR item_name[MAX_STRING_SIZE + 4] = {0};
	TCHAR format_string[MAX_STRING_SIZE] = {0};

	HTREEITEM cur_tree_item = NULL;
	HTREEITEM cur_tree_item2 = NULL;

	LPTSTR file_name = NULL;

	treeview_clear();
	segd_file_close(__read_segd_file);
	clear_treeview_item_data_list();

	__matrix_zero_Memory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = window_handle;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = __TEXT("SGD文件格式\0*.SEGD\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn)) return;

	file_name = PathFindFileName(ofn.lpstrFile);
	TCHAR* xx = (TCHAR*)malloc(260);
	get_string(__theApp.instance_, IDS_STRING124, format_string, sizeof(format_string));
	wsprintf(item_name, format_string, file_name);
	treeview_add_root(item_name, NULL);

	get_string(__theApp.instance_, IDS_STRING125, format_string, sizeof(format_string));
	treeview_add_item(format_string, NULL, treeview_get_last_item_inserted());
	cur_tree_item = treeview_get_the_last_item_inserted();

	__read_segd_file = segd_file_create(ofn.lpstrFile, SEGD_FILE_OPEN_EXISTING | SEGD_FILE_IO_OVERLAPPED);

	if(__read_segd_file == NULL){

		show_message_box(__theApp.instance_, window_handle, IDS_STRING104, MB_ICONERROR);
		return;
	}

	//general header #1
	is_ok = segd_file_read_genernal_header_1(__read_segd_file, &__read_segd_file->header_1_);
	if(!is_ok){

		err = GetLastError();
		show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
		return;
	}

	get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
	wsprintf(item_name, format_string, 1);
	treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
		add_treeview_item_data_into_list(SEGD_GENERAL_HEADER_1_TYPE, &__read_segd_file->header_1_));

	//general header #2
	is_ok = segd_file_read_genernal_header_2(__read_segd_file, &__read_segd_file->header_2_);
	if(!is_ok){

		err = GetLastError();
		show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
		return;
	}

	get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
	wsprintf(item_name, format_string, 2);
	treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
		add_treeview_item_data_into_list(SEGD_GENERAL_HEADER_2_TYPE, &__read_segd_file->header_2_));

	//general header #3
	if(__read_segd_file->header_1_.additional_blocks_in_general_header_ == 2){

		is_ok = segd_file_read_genernal_header_3(__read_segd_file, &__read_segd_file->header_3_);
		if(!is_ok){

			err = GetLastError();
			show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
			return;
		}

		get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
		wsprintf(item_name, format_string, 3);
		treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
			add_treeview_item_data_into_list(SEGD_GENERAL_HEADER_3_TYPE, &__read_segd_file->header_3_));
	}

	//scan type header #
	if(__read_segd_file->header_1_.channal_sets_scan_type_ > 0){

		size = __read_segd_file->header_1_.channal_sets_scan_type_ * sizeof(SEGD_SCAN_TYPE_HEADER);
		__read_segd_file->scan_type_header_ = (SEGD_SCAN_TYPE_HEADER_PTR)malloc(size);
		__matrix_zero_Memory(__read_segd_file->scan_type_header_, size);

		get_string(__theApp.instance_, IDS_STRING130, format_string, sizeof(format_string));
		treeview_add_item(format_string, treeview_get_root_item(), treeview_get_last_item_inserted());
		cur_tree_item = treeview_get_the_last_item_inserted();

		for(i = 0; i < __read_segd_file->header_1_.channal_sets_scan_type_; ++i){

			is_ok = segd_file_read_scan_type_header(__read_segd_file, __read_segd_file->scan_type_header_ + i);
			if(!is_ok){

				err = GetLastError();
				show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
				return;
			}

			//add to treeview
			get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
			wsprintf(item_name, format_string, i + 1);
			treeview_add_item(item_name, cur_tree_item, treeview_get_the_last_item_inserted(), 
				add_treeview_item_data_into_list(SEGD_SCAN_TYPE_HEADER_TYPE, __read_segd_file->scan_type_header_ + i));
		}
	}

	//extended header
	if(__read_segd_file->header_1_.extended_header_block_ == 32){

		//general header
		is_ok = segd_file_read_extended_header(__read_segd_file, &__read_segd_file->extended_header_);
		if(!is_ok){

			err = GetLastError();
			show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
			return;
		}

		//add to treeview
		get_string(__theApp.instance_, IDS_STRING131, format_string, sizeof(format_string));
		treeview_add_item(format_string, treeview_get_root_item(), treeview_get_last_item_inserted(), 
			add_treeview_item_data_into_list(SEGD_EXTENDED_HEADER_TYPE, &__read_segd_file->extended_header_));
	}

	//external header
	if(__read_segd_file->header_1_.external_header_block_ > 0){

		size = __read_segd_file->header_1_.external_header_block_ * sizeof(SEGD_UCHAR) * 32;
		__read_segd_file->external_header_.series_ = (SEGD_UCHAR*)malloc(size);
		__matrix_zero_Memory(__read_segd_file->external_header_.series_, size);

		//read it
		is_ok = segd_file_read_external_header(__read_segd_file, size, &__read_segd_file->external_header_);
		if(!is_ok){

			err = GetLastError();
			show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
			return;
		}

		//add to treeview
		get_string(__theApp.instance_, IDS_STRING132, format_string, sizeof(format_string));
		treeview_add_item(format_string, treeview_get_root_item(), treeview_get_last_item_inserted(), 
			add_treeview_item_data_into_list(SEGD_EXTERNAL_HEADER_TYPE, &__read_segd_file->external_header_));
	}


	if(__read_segd_file->header_1_.extended_header_block_ == 32){

		//trace header + data
		if(__read_segd_file->extended_header_.trace_number_ > 0){

			i = __read_segd_file->extended_header_.trace_number_ * sizeof(SEGD_FILE_TRACE_HEADER);
			__read_segd_file->trace_headers_ = (SEGD_FILE_TRACE_HEADER_PTR)malloc(i);
			__matrix_zero_Memory(__read_segd_file->trace_headers_, i);
			//trace
			get_string(__theApp.instance_, IDS_STRING133, format_string, sizeof(format_string));
			treeview_add_item(format_string, treeview_get_root_item(), treeview_get_last_item_inserted());
			cur_tree_item2 = cur_tree_item = treeview_get_the_last_item_inserted();
		}

		for(i = 0; i < __read_segd_file->extended_header_.trace_number_; ++i){

			is_ok = segd_file_read_trace_header(__read_segd_file, __read_segd_file->trace_headers_ + i);
			if(!is_ok){

				err = GetLastError();
				show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
				return;
			}

			//segd data
			if(__read_segd_file->trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

				size = segd_file_get_trace_data_size(__read_segd_file);
				__read_segd_file->trace_headers_[i].trace_data_ = (SEGD_UCHAR*)malloc(size);
				segd_file_read_trace_data(__read_segd_file, __read_segd_file->trace_headers_[i].trace_data_, size);
			}

			//add treeview trace header
			get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
			wsprintf(item_name, format_string, i + 1);
			treeview_add_item(item_name, cur_tree_item2, treeview_get_the_last_item_inserted());
			cur_tree_item = treeview_get_the_last_item_inserted();

			//add treeview:	dumex trace header
			get_string(__theApp.instance_, IDS_STRING134, format_string, sizeof(format_string));
			treeview_add_item(format_string, cur_tree_item, treeview_get_last_item_inserted(), 
				add_treeview_item_data_into_list(SEGD_DEMUX_TRACE_HEADER_TYPE, &__read_segd_file->trace_headers_[i].demux_trace_header_));

			//add treeview: extended trace header
			get_string(__theApp.instance_, IDS_STRING135, format_string, sizeof(format_string));
			treeview_add_item(format_string, cur_tree_item, treeview_get_last_item_inserted());
			cur_tree_item = treeview_get_the_last_item_inserted();

			for(int j = 1; j <= __read_segd_file->trace_headers_[i].demux_trace_header_.trace_header_extension_; ++j){

				get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
				wsprintf(item_name, format_string, j);
				switch(j){
				case 1:
					treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
						add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_1_TYPE, 
							&__read_segd_file->trace_headers_[i].trace_header_extended_1_));
					break;
				case 2:
					treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
						add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_2_TYPE, 
							&__read_segd_file->trace_headers_[i].trace_header_extended_2_));
					break;
				case 3:
					treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
						add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_3_TYPE, 
							&__read_segd_file->trace_headers_[i].trace_header_extended_3_));
					break;
				case 4:
					treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
						add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_4_TYPE, 
							&__read_segd_file->trace_headers_[i].trace_header_extended_4_));
					break;
				case 5:
					treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
						add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_5_TYPE, 
							&__read_segd_file->trace_headers_[i].trace_header_extended_5_));
					break;
				case 6:
					treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
						add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_6_TYPE, 
							&__read_segd_file->trace_headers_[i].trace_header_extended_6_));
					break;
				case 7:
					treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
						add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_7_TYPE, 
							&__read_segd_file->trace_headers_[i].trace_header_extended_7_));
					break;
				}
			}
		}


		//aux trace header + data
		if(__read_segd_file->extended_header_.auxiliary_trace_number_ > 0){

			size = __read_segd_file->extended_header_.auxiliary_trace_number_ * sizeof(SEGD_FILE_TRACE_HEADER);
			__read_segd_file->aux_trace_headers_ = (SEGD_FILE_TRACE_HEADER_PTR)malloc(size);
			__matrix_zero_Memory(__read_segd_file->aux_trace_headers_, size);

			get_string(__theApp.instance_, IDS_STRING139, format_string, sizeof(format_string));
			treeview_add_item(format_string, treeview_get_root_item(), treeview_get_last_item_inserted());
			cur_tree_item2 = cur_tree_item = treeview_get_the_last_item_inserted();

			for(i = 0; i < __read_segd_file->extended_header_.auxiliary_trace_number_; ++i){

				is_ok = segd_file_read_trace_header(__read_segd_file, __read_segd_file->aux_trace_headers_ + i);
				if(!is_ok){

					err = GetLastError();
					show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
					return;
				}

				//trace data
				if(__read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

					size = segd_file_get_trace_data_size(__read_segd_file);
					__read_segd_file->aux_trace_headers_[i].trace_data_ = (SEGD_UCHAR*)malloc(size);
					segd_file_read_trace_data(__read_segd_file, __read_segd_file->aux_trace_headers_[i].trace_data_, size);
				}

				//add treeview aux trace header
				get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
				wsprintf(item_name, format_string, i + 1);
				treeview_add_item(item_name, cur_tree_item2, treeview_get_the_last_item_inserted());
				cur_tree_item = treeview_get_the_last_item_inserted();

				//add treeview item
				get_string(__theApp.instance_, IDS_STRING134, format_string, sizeof(format_string));
				treeview_add_item(format_string, cur_tree_item, treeview_get_last_item_inserted(), 
					add_treeview_item_data_into_list(SEGD_DEMUX_TRACE_HEADER_TYPE, 
						&__read_segd_file->aux_trace_headers_[i].demux_trace_header_));

				get_string(__theApp.instance_, IDS_STRING135, format_string, sizeof(format_string));
				treeview_add_item(format_string, cur_tree_item, treeview_get_last_item_inserted());
				cur_tree_item = treeview_get_the_last_item_inserted();

				for(int j = 1; j <= __read_segd_file->aux_trace_headers_[i].demux_trace_header_.trace_header_extension_; ++j){

					get_string(__theApp.instance_, IDS_STRING126, format_string, sizeof(format_string));
					wsprintf(item_name, format_string, j);
					switch(j){
					case 1:
						treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
							add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_1_TYPE, 
								&__read_segd_file->aux_trace_headers_[i].trace_header_extended_1_));
						break;
					case 2:
						treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
							add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_2_TYPE, 
								&__read_segd_file->aux_trace_headers_[i].trace_header_extended_2_));
						break;
					case 3:
						treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
							add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_3_TYPE, 
								&__read_segd_file->aux_trace_headers_[i].trace_header_extended_3_));
						break;
					case 4:
						treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
							add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_4_TYPE, 
								&__read_segd_file->aux_trace_headers_[i].trace_header_extended_4_));
						break;
					case 5:
						treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
							add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_5_TYPE, 
								&__read_segd_file->aux_trace_headers_[i].trace_header_extended_5_));
						break;
					case 6:
						treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
							add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_6_TYPE, 
								&__read_segd_file->aux_trace_headers_[i].trace_header_extended_6_));
						break;
					case 7:
						treeview_add_item(item_name, cur_tree_item,	treeview_get_last_item_inserted(), 
							add_treeview_item_data_into_list(SEGD_TRACE_EXTENDED_HEADER_7_TYPE, 
								&__read_segd_file->aux_trace_headers_[i].trace_header_extended_7_));
						break;
					}
				}
			}			
		}
	}

	int j = 0;
	HANDLE handle = 0x0;
	DWORD buf_size_dat = 0x0;
	char buf_file_name[MAX_PATH] = "segd.txt";
	float f_val = 0.0f;
	UCHAR data[4] = {0x0};
	char buf[MAX_PATH] = {0x0};

	handle = CreateFileA(buf_file_name, FILE_ALL_ACCESS, FILE_SHARE_WRITE, NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL, NULL);

	wcstombs(buf_file_name, __read_segd_file->path_name_, wcslen(__read_segd_file->path_name_) + 1);
	sprintf_s(buf, sizeof(buf), "SEGD File: %s\n", buf_file_name);
	WriteFile(handle, buf, strlen(buf) + 1, &buf_size_dat, NULL);

	strcpy(buf,"SEGD_GENERAL_HEADER 1:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "file_number: %d\n", __read_segd_file->header_1_.file_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "format_code: %d\n", __read_segd_file->header_1_.format_code_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "general_constants: %s\n", __read_segd_file->header_1_.general_constants_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "year: %d\n", __read_segd_file->header_1_.year_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "additional_blocks_in_general_header: %d\n", __read_segd_file->header_1_.additional_blocks_in_general_header_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "day: %d\n", __read_segd_file->header_1_.day_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "hour: %d\n", __read_segd_file->header_1_.hour_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "minute: %d\n", __read_segd_file->header_1_.minute_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "second: %d\n", __read_segd_file->header_1_.second_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "manufacturer_code: %d\n", __read_segd_file->header_1_.manufacturer_code_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "manufacturer_serial_number: %d\n", __read_segd_file->header_1_.manufacturer_serial_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "bytes_per_scan: %d\n", __read_segd_file->header_1_.bytes_per_scan_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "base_scan_interval: %d\n", __read_segd_file->header_1_.base_scan_interval_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "polarity: %d\n", __read_segd_file->header_1_.polarity_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "record_type: %d\n", __read_segd_file->header_1_.record_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "record_length: %d\n", __read_segd_file->header_1_.record_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "scan_types_record: %d\n", __read_segd_file->header_1_.scan_types_record_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "channal_sets_scan_type: %d\n", __read_segd_file->header_1_.channal_sets_scan_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "skew_blocks: %d\n", __read_segd_file->header_1_.skew_blocks_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "extended_header_block: %d\n", __read_segd_file->header_1_.extended_header_block_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "external_header_block: %d\n", __read_segd_file->header_1_.external_header_block_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	//header2
	strcpy(buf,"\nSEGD_GENERAL_HEADER 2:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "expanded_file_number: %d\n", __read_segd_file->header_2_.expanded_file_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "extended_channel_sets_scan_type: %d\n", __read_segd_file->header_2_.extended_channel_sets_scan_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "major_segd_revision_number: %d\n", __read_segd_file->header_2_.major_segd_revision_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "minor_segd_revision_number: %d\n", __read_segd_file->header_2_.minor_segd_revision_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "number_of_blocks_of_general_trailer: %d\n", __read_segd_file->header_2_.number_of_blocks_of_general_trailer_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "extended_record_length: %d\n", __read_segd_file->header_2_.extended_record_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "general_header_block_number: %d\n", __read_segd_file->header_2_.general_header_block_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	//header3
	strcpy(buf,"\nSEGD_GENERAL_HEADER 3:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "expanded_file_number: %d\n", __read_segd_file->header_3_.expanded_file_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_line_number: %f\n", __read_segd_file->header_3_.source_line_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_point_number: %f\n", __read_segd_file->header_3_.source_point_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_point_index: %d\n", __read_segd_file->header_3_.source_point_index_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_point_index: %d\n", __read_segd_file->header_3_.source_point_index_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "phase_control: %d\n", __read_segd_file->header_3_.phase_control_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "type_vibrator: %d\n", __read_segd_file->header_3_.type_vibrator_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "phase_angle: %d\n", __read_segd_file->header_3_.phase_angle_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "general_header_block_number: %d\n", __read_segd_file->header_3_.general_header_block_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_set_number: %d\n", __read_segd_file->header_3_.source_set_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	//scan type header
	for(i = 0; i < 16; ++i){

		sprintf_s(buf, sizeof(buf), "\nSEGD_SCAN_TYPE_HEADER #%d :\n", i + 1);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "scan_type_number: %d\n", (__read_segd_file->scan_type_header_ + i)->scan_type_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_set_number: %d\n", (__read_segd_file->scan_type_header_ + i)->channel_set_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_set_start_time: %d\n", (__read_segd_file->scan_type_header_ + i)->channel_set_start_time_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_set_end_time: %d\n", (__read_segd_file->scan_type_header_ + i)->channel_set_end_time_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "descale_multiplier: %d\n", (__read_segd_file->scan_type_header_ + i)->descale_multiplier_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channels_number: %d\n", (__read_segd_file->scan_type_header_ + i)->channels_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_type_identification: %d\n", (__read_segd_file->scan_type_header_ + i)->channel_type_identification_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "sample_channel_subscans_number: %d\n", (__read_segd_file->scan_type_header_ + i)->sample_channel_subscans_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_gain_control: %d\n", (__read_segd_file->scan_type_header_ + i)->channel_gain_control_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "alias_filter_frequency: %d\n", (__read_segd_file->scan_type_header_ + i)->alias_filter_frequency_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "alias_filter_slope: %d\n", (__read_segd_file->scan_type_header_ + i)->alias_filter_slope_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "low_cut_filter: %d\n", (__read_segd_file->scan_type_header_ + i)->low_cut_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "low_cut_filter_slope: %d\n", (__read_segd_file->scan_type_header_ + i)->low_cut_filter_slope_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "first_notch_filter: %d\n", (__read_segd_file->scan_type_header_ + i)->first_notch_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "second_notch_filter: %d\n", (__read_segd_file->scan_type_header_ + i)->second_notch_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "third_notch_filter: %d\n", (__read_segd_file->scan_type_header_ + i)->third_notch_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "extended_channel_set_number: %d\n", (__read_segd_file->scan_type_header_ + i)->extended_channel_set_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "extended_header_flag: %d\n", (__read_segd_file->scan_type_header_ + i)->extended_header_flag_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "trace_header_extension: %d\n", (__read_segd_file->scan_type_header_ + i)->trace_header_extension_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "vertical_stack: %d\n", (__read_segd_file->scan_type_header_ + i)->vertical_stack_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "streamer_number: %d\n", (__read_segd_file->scan_type_header_ + i)->streamer_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "array_forming: %d\n", (__read_segd_file->scan_type_header_ + i)->array_forming_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
	}

	
	strcpy(buf,"\nSEGD_EXTENDED_HEADER:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_length: %d\n", __read_segd_file->extended_header_.sample_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "sample_frequency: %d\n", __read_segd_file->extended_header_.sample_frequency_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "trace_total_number: %d\n", __read_segd_file->extended_header_.trace_total_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "auxiliary_trace_number: %d\n", __read_segd_file->extended_header_.auxiliary_trace_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "trace_number: %d\n", __read_segd_file->extended_header_.trace_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "active_trace_number: %d\n", __read_segd_file->extended_header_.active_trace_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_type: %d\n", __read_segd_file->extended_header_.source_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_number_in_trace: %d\n", __read_segd_file->extended_header_.sample_number_in_trace_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "shot_number: %d\n", __read_segd_file->extended_header_.shot_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tb_window: %d\n", __read_segd_file->extended_header_.tb_window_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "test_type: %d\n", __read_segd_file->extended_header_.test_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_first_test: %d\n", __read_segd_file->extended_header_.line_first_test_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_first_number: %d\n", __read_segd_file->extended_header_.line_first_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "array_number: %d\n", __read_segd_file->extended_header_.array_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "array_type: %d\n", __read_segd_file->extended_header_.array_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "time_break_signal: %d\n", __read_segd_file->extended_header_.time_break_signal_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "well_time: %d\n", __read_segd_file->extended_header_.well_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "blaster_serial_number: %d\n", __read_segd_file->extended_header_.blaster_serial_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "blaster_status: %d\n", __read_segd_file->extended_header_.blaster_status_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "refraction_delay: %d\n", __read_segd_file->extended_header_.refraction_delay_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tb_to_t0_time: %d\n", __read_segd_file->extended_header_.tb_to_t0_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "internal_time_break_signal: %d\n", __read_segd_file->extended_header_.internal_time_break_signal_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "field_unit_prestack: %d\n", __read_segd_file->extended_header_.field_unit_prestack_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "Noise_elimination_type: %d\n", __read_segd_file->extended_header_.Noise_elimination_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "low_trace_percent: %d\n", __read_segd_file->extended_header_.low_trace_percent_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "low_trace_value: %d\n", __read_segd_file->extended_header_.low_trace_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "window_number: %d\n", __read_segd_file->extended_header_.window_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_edit_type: %d\n", __read_segd_file->extended_header_.historical_edit_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "noisy_trace_percent: %d\n", __read_segd_file->extended_header_.noisy_trace_percent_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_range: %d\n", __read_segd_file->extended_header_.historical_range_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_taper_length: %d\n", __read_segd_file->extended_header_.historical_taper_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "threshold_changed: %d\n", __read_segd_file->extended_header_.threshold_changed_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_threshold_initial_value: %d\n", __read_segd_file->extended_header_.historical_threshold_initial_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_zeroing_length: %d\n", __read_segd_file->extended_header_.historical_zeroing_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	
	sprintf_s(buf, sizeof(buf), "process_type: %d\n", __read_segd_file->extended_header_.process_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	sprintf_s(buf, sizeof(buf), "overlap_number: %d\n", __read_segd_file->extended_header_.overlap_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "record_length: %d\n", __read_segd_file->extended_header_.record_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "autocorrelative_peak_time: %d\n", __read_segd_file->extended_header_.autocorrelative_peak_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "related_reference_signal_number: %d\n", __read_segd_file->extended_header_.related_reference_signal_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "reference_signal_length: %d\n", __read_segd_file->extended_header_.reference_signal_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "scan_length: %d\n", __read_segd_file->extended_header_.scan_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_number: %d\n", __read_segd_file->extended_header_.sample_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "max_auxiliary_channel_max_value: %f\n", __read_segd_file->extended_header_.max_auxiliary_channel_max_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "max_channel_max_value: %f\n", __read_segd_file->extended_header_.max_channel_max_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "dump_overlap_number: %d\n", __read_segd_file->extended_header_.dump_overlap_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tape_lable: %s\n", __read_segd_file->extended_header_.tape_lable_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tape_number_: %d\n", __read_segd_file->extended_header_.tape_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "version: %d\n", __read_segd_file->extended_header_.version_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "date: %d\n", __read_segd_file->extended_header_.date_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_easting: %f\n", __read_segd_file->extended_header_.source_easting_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_northing_: %f\n", __read_segd_file->extended_header_.source_northing_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "Source_elevation: %d\n", __read_segd_file->extended_header_.Source_elevation_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "slip_sweep_mode: %d\n", __read_segd_file->extended_header_.slip_sweep_mode_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	SEGD_UINT	file_number_per_tape_;		//597-600 	XXXX 	bin	每个磁带的文件数	1 到 9999 	 
	sprintf_s(buf, sizeof(buf), "file_number_per_tape: %d\n", __read_segd_file->extended_header_.file_number_per_tape_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "file_counter: %d\n", __read_segd_file->extended_header_.file_counter_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_error_explaination: %d\n", __read_segd_file->extended_header_.sample_error_explaination_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "filter_type: %d\n", __read_segd_file->extended_header_.filter_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "stack_dumped: %d\n", __read_segd_file->extended_header_.stack_dumped_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "stack_symbol: %d\n", __read_segd_file->extended_header_.stack_symbol_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "prm_tilt_correction: %d\n", __read_segd_file->extended_header_.prm_tilt_correction_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_name: %s\n", __read_segd_file->extended_header_.line_name_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "operation_mode: %d\n", __read_segd_file->extended_header_.operation_mode_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "non_log: %d\n", __read_segd_file->extended_header_.non_log_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "listen_time: %d\n", __read_segd_file->extended_header_.listen_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "dump_type: %d\n", __read_segd_file->extended_header_.dump_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_id: %d\n", __read_segd_file->extended_header_.line_id_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


	for(i = 0; i < __read_segd_file->extended_header_.trace_number_; ++i){

		sprintf_s(buf, sizeof(buf), "\n记录道数据块#%d :\n", i);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


		strcpy(buf, "\n多路解码道头:\n");
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "file_number: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.file_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "scan_type_number: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.scan_type_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_set_number: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.channel_set_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "trace_number: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.trace_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "first_timing_word: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.first_timing_word_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "trace_header_extension: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.trace_header_extension_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "sample_skew: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.sample_skew_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "trace_edit: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.trace_edit_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "time_break_window: %f\n", __read_segd_file->trace_headers_[i].demux_trace_header_.time_break_window_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "extended_channel_set_number: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.extended_channel_set_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "extended_file_number: %d\n", __read_segd_file->trace_headers_[i].demux_trace_header_.extended_file_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		for(j = 0; j < __read_segd_file->scan_type_header_[0].trace_header_extension_; ++j){

			switch(j){
				case 0:
					{

						sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_line_number: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_1_.receiver_line_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_number: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_1_.receiver_point_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_index: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_1_.receiver_point_index_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "number_of_samples_per_trace: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "extended_receiver_line_number: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_1_.extended_receiver_line_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "extended_receiver_point_number: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_1_.extended_receiver_point_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "sensor_type: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_1_.sensor_type_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

					}
					break;
				case 1:
					{
						sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_east_: %f\n", __read_segd_file->trace_headers_[i].trace_header_extended_2_.receiver_point_east_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_north: %f\n", __read_segd_file->trace_headers_[i].trace_header_extended_2_.receiver_point_north_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
						
						sprintf_s(buf, sizeof(buf), "receiver_point_high: %f\n", __read_segd_file->trace_headers_[i].trace_header_extended_2_.receiver_point_high_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
						
						sprintf_s(buf, sizeof(buf), "detector_type_number: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_2_.detector_type_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
						
						sprintf_s(buf, sizeof(buf), "extended_trace_number: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_2_.extended_trace_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
					}
					break;
				case 2:
					{
						sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						
						sprintf_s(buf, sizeof(buf), "resistor_lower_limit: %f\n", __read_segd_file->trace_headers_[i].trace_header_extended_3_.resistor_lower_limit_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						
						sprintf_s(buf, sizeof(buf), "resistor_higher_limit: %f\n", __read_segd_file->trace_headers_[i].trace_header_extended_3_.resistor_higher_limit_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						
						sprintf_s(buf, sizeof(buf), "resistor_value: %f\n", __read_segd_file->trace_headers_[i].trace_header_extended_3_.resistor_value_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						
						sprintf_s(buf, sizeof(buf), "tilt_limit: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_3_.tilt_limit_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						
						sprintf_s(buf, sizeof(buf), "tilt_value_: %f\n", __read_segd_file->trace_headers_[i].trace_header_extended_3_.tilt_value_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						
						sprintf_s(buf, sizeof(buf), "resistor_deviation: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_3_.resistor_deviation_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						
						sprintf_s(buf, sizeof(buf), "tilt_deviation: %d\n", __read_segd_file->trace_headers_[i].trace_header_extended_3_.tilt_deviation_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
					}
					break;
			}
		}
	
		//read data
		size = segd_file_get_trace_data_size(__read_segd_file);
		for(j = 0; j < size; j += CURRENT_SEGD_MINI_DATA_LENGTH){

			data[3] = *(__read_segd_file->trace_headers_[i].trace_data_ + j);
			data[2] = *(__read_segd_file->trace_headers_[i].trace_data_ + j + 1);
			data[1] = *(__read_segd_file->trace_headers_[i].trace_data_ + j + 2);
			data[0] = *(__read_segd_file->trace_headers_[i].trace_data_ + j + 3);
			memcpy(&f_val, data, sizeof(UCHAR) * CURRENT_SEGD_MINI_DATA_LENGTH);
			memset(buf_file_name, 0x0, sizeof(buf_file_name));
			sprintf_s(buf_file_name, sizeof(buf_file_name), "%f,", f_val);
			WriteFile(handle, buf_file_name, strlen(buf_file_name), &buf_size_dat, NULL);
		}
	}

	strcpy(buf,"\nSEGD_FILE_AUX_TRACE_HEADER:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
	if(__read_segd_file->extended_header_.auxiliary_trace_number_ > 0){

		for(i = 0; i < __read_segd_file->extended_header_.auxiliary_trace_number_; ++i){

			sprintf_s(buf, sizeof(buf), "\n辅助记录道数据块#%d :\n", i);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			strcpy(buf, "\n多路解码道头:\n");
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "file_number: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.file_number_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "scan_type_number: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.scan_type_number_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "channel_set_number: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.channel_set_number_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "trace_number: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.trace_number_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "first_timing_word: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.first_timing_word_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "trace_header_extension: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.trace_header_extension_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "sample_skew: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.sample_skew_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "trace_edit: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.trace_edit_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "time_break_window: %f\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.time_break_window_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "extended_channel_set_number: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.extended_channel_set_number_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			sprintf_s(buf, sizeof(buf), "extended_file_number: %d\n", __read_segd_file->aux_trace_headers_[i].demux_trace_header_.extended_file_number_);
			WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

			for(j = 0; j < __read_segd_file->scan_type_header_[0].trace_header_extension_; ++j){

				switch(j){
				case 0:
					{

						sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_line_number: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.receiver_line_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_number: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.receiver_point_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_index: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.receiver_point_index_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "number_of_samples_per_trace: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "extended_receiver_line_number: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.extended_receiver_line_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "extended_receiver_point_number: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.extended_receiver_point_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "sensor_type: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_1_.sensor_type_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

					}
					break;
				case 1:
					{
						sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_east_: %f\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_2_.receiver_point_east_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_north: %f\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_2_.receiver_point_north_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "receiver_point_high: %f\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_2_.receiver_point_high_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "detector_type_number: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_2_.detector_type_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "extended_trace_number: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_2_.extended_trace_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
					}
					break;
				case 2:
					{
						sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


						sprintf_s(buf, sizeof(buf), "resistor_lower_limit: %f\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_3_.resistor_lower_limit_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


						sprintf_s(buf, sizeof(buf), "resistor_higher_limit: %f\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_3_.resistor_higher_limit_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


						sprintf_s(buf, sizeof(buf), "resistor_value: %f\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_3_.resistor_value_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


						sprintf_s(buf, sizeof(buf), "tilt_limit: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_3_.tilt_limit_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


						sprintf_s(buf, sizeof(buf), "tilt_value_: %f\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_3_.tilt_value_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


						sprintf_s(buf, sizeof(buf), "resistor_deviation: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_3_.resistor_deviation_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


						sprintf_s(buf, sizeof(buf), "tilt_deviation: %d\n", __read_segd_file->aux_trace_headers_[i].trace_header_extended_3_.tilt_deviation_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
					}
					break;
				}
			}

			//read data
			size = segd_file_get_trace_data_size(__read_segd_file);
			for(j = 0; j < size; j+=CURRENT_SEGD_MINI_DATA_LENGTH){

				data[3] = *(__read_segd_file->aux_trace_headers_[i].trace_data_ + j);
				data[2] = *(__read_segd_file->aux_trace_headers_[i].trace_data_ + j + 1);
				data[1] = *(__read_segd_file->aux_trace_headers_[i].trace_data_ + j + 2);
				data[0] = *(__read_segd_file->aux_trace_headers_[i].trace_data_ + j + 3);
				memcpy(&f_val, data, sizeof(UCHAR) * CURRENT_SEGD_MINI_DATA_LENGTH);
				memset(buf_file_name, 0x0, sizeof(buf_file_name));
				sprintf_s(buf_file_name, sizeof(buf_file_name), "%f,", f_val);
				WriteFile(handle, buf_file_name, strlen(buf_file_name), &buf_size_dat, NULL);
			}
		}
	}

	CloseHandle(handle);
}

void on_save_file(HWND window_handle)
{
	int i = 0, size = 0;
	DWORD err = 0x0;
	bool is_ok = true;

	OPENFILENAME ofn;

	TCHAR szFile[MAX_STRING_SIZE + 4] = {0};

	SEGD_FILE_PTR write_segd_file = NULL;
	SEGD_FILE_PTR segd_file = __read_segd_file;

	__matrix_zero_Memory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = window_handle;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = __TEXT("SGD文件格式\0*.SGD\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetSaveFileName(&ofn)) return ;

	write_segd_file = segd_file_create(ofn.lpstrFile, SEGD_FILE_CREATE);

	if(write_segd_file == NULL){

		show_message_box(__theApp.instance_, window_handle, IDS_STRING104, MB_ICONERROR);
		return;
	}
	
	//segd_file->header_2_.major_segd_revision_number_ = 2;
	//segd_file->header_2_.external_header_blocks_ = 0;
	//segd_file->header_3_.source_set_number_ = 1;
	//segd_file->scan_type_header_->trace_header_extension_ = 3;
	//segd_file->scan_type_header_[1].trace_header_extension_ = 3;
	//segd_file->extended_header_.sample_number_in_trace_ = 2001;

	//for(i = 0; i < segd_file->extended_header_.trace_number_; ++i){
	//	segd_file->trace_headers_[i].demux_trace_header_.trace_header_extension_ = 3;
	//	segd_file->trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ = 2001;
	//}

	//for(i = 0; i < segd_file->extended_header_.auxiliary_trace_number_; ++i){
	//	segd_file->aux_trace_headers_[i].demux_trace_header_.trace_header_extension_ = 3;
	//	segd_file->aux_trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ = 2001;
	//}

	is_ok = segd_file_write_genernal_header_1(write_segd_file, &segd_file->header_1_);
	if(!is_ok){

		err = GetLastError();
		show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
		return;
	}

	is_ok = segd_file_write_genernal_header_2(write_segd_file, &segd_file->header_2_);
	if(!is_ok){

		err = GetLastError();
		show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
		return;
	}

	if(segd_file->header_1_.additional_blocks_in_general_header_ == 2){

		is_ok = segd_file_write_genernal_header_3(write_segd_file, &segd_file->header_3_);
		if(!is_ok){

			err = GetLastError();
			show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
			return;
		}
	}

	for(i = 0; i < segd_file->header_1_.channal_sets_scan_type_; ++i){

		is_ok = segd_file_write_scan_type_header(write_segd_file, segd_file->scan_type_header_ + i);
		if(!is_ok){

			err = GetLastError();
			show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
			return;
		}
	}

	//extended header
	if(segd_file->header_1_.extended_header_block_ == 32){

		is_ok = segd_file_write_extended_header(write_segd_file, &segd_file->extended_header_);
		if(!is_ok){

			err = GetLastError();
			show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
			return;
		}
	}

	//external header
	if(segd_file->header_1_.external_header_block_ > 0){

		is_ok = segd_file_write_external_header(write_segd_file, 
												segd_file->header_1_.external_header_block_ * sizeof(UCHAR) * 32, 
												&segd_file->external_header_);
		if(!is_ok){

			err = GetLastError();
			show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
			return;
		}
	}


	if(segd_file->header_1_.extended_header_block_ == 32){

		//trace header + data
		for(i = 0; i < segd_file->extended_header_.trace_number_; ++i){

			is_ok = segd_file_write_trace_header(write_segd_file, segd_file->trace_headers_ + i);
			if(!is_ok){

				err = GetLastError();
				show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
				return;
			}

			//segd data
			if(segd_file->trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

				size = segd_file_get_trace_data_size(segd_file);
				segd_file_write_trace_data(write_segd_file, segd_file->trace_headers_[i].trace_data_, size);
			}
		}

		//aux trace header + data
		if(segd_file->extended_header_.auxiliary_trace_number_ > 0){

			for(i = 0; i < segd_file->extended_header_.auxiliary_trace_number_; ++i){

				is_ok = segd_file_write_trace_header(write_segd_file, segd_file->aux_trace_headers_ + i);
				if(!is_ok){

					err = GetLastError();
					show_message_box(__theApp.instance_, window_handle, err, MB_ICONERROR);
					return;
				}

				if(segd_file->aux_trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

					size = segd_file_get_trace_data_size(segd_file);
					segd_file_write_trace_data(write_segd_file, segd_file->aux_trace_headers_[i].trace_data_, size);
				}
			}			
		}
	}

	segd_file_close(write_segd_file);
}

