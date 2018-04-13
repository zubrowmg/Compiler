void CodeGen::generalExpression(list temp_list2){
	tokens tok_temp2; int count = 1; list new_list; bool last_T_LBRACKET = false;
	


	new_list.reset_pos(); int and_or = 0; tokens temp_and_or;
	for (int j = 0; j < new_list.get_size(); j++){
		tok_temp2 = new_list.get_one();
		if (count == 1){
			myfile2 << "R[" << and_or << "].bool_val=";
			count = count + 1;
		} else {

		}

		new_list = outputMainNew(new_list, tok_temp2);
		
		if (tok_temp2.type == "T_OR" || tok_temp2.type == "T_AND"){
			temp_and_or = tok_temp2;
			and_or = and_or + 1;
			count = 1;
		} else if (j == new_list.get_size() - 1){
			and_or = and_or + 1;
		}

		if (j == new_list.get_size() - 1 || tok_temp2.type == "T_OR" || tok_temp2.type == "T_AND"){
			myfile2 << ";" << "\n" << "\t";
			if (and_or >= 2){
				myfile2 << "R[" << and_or - 2 << "].bool_val=";
				myfile2 << "R[" << and_or - 2 << "].bool_val";
				if (temp_and_or.type == "T_OR"){
					myfile2 << "||";
				} else if (temp_and_or.type == "T_AND"){
					myfile2 << "&&";
				}
				myfile2 << "R[" << and_or - 1 << "].bool_val";
				myfile2 << ";" << "\n" << "\t";
				and_or = 1;
			} 

			
		}
	}
}