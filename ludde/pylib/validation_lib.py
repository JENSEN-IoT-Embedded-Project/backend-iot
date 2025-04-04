import re


def is_email_valid(email) :
    valid_pattern = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'
    if re.match(valid_pattern, email) :
        return True
    else :
        return False

def is_number_valid(phone):
    se_mobile_pattern = r'^(\+46|0)7\d{8}$'
    if re.match(se_mobile_pattern, phone) :
        return True
    else :
        return False
