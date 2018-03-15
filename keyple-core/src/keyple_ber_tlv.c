
int parseTlv(const unsigned char *buffer, int length, void* ctx, int(*tagCallback)(void* ctx, int tag, int length, const unsigned char* value)) {
	while (length>0) {
		// Get tag
		int tag = *(buffer++);
		int tagLength, tmp;
		length--;
		if ((tag & 0x1F) == 0x1F) {
			if ((length--) == 0) return -1;
			tag = (tag << 8) | *(buffer++);
			if ((tag & 0x80) == 0x80) {
				if ((length--) == 0) return -1;
				tag = (tag << 8) | *(buffer++);
				if ((tag & 0x80) == 0x80) {
					if ((length--) == 0) return -1;
					tag = (tag << 8) | *(buffer++);
					if ((tag & 0x80) == 0x80) {
						// Longer tags are NOT supported
						return -1;
					}
				}
			}
		}
		else {
			if (tag == 0) {
				continue;
			}
		}

		// Get length
		if ((length--) == 0) return -1;
		tmp = *(buffer++);
		tagLength = 0;

		switch (tmp) {
		case 0x84:
			if ((length--) == 0) return -1;
			tagLength = *(buffer++);
			/* no break */
		case 0x83:
			if ((length--) == 0) return -1;
			tagLength = (tagLength << 8) | *(buffer++);
			/* no break */
		case 0x82:
			if ((length--) == 0) return -1;
			tagLength = (tagLength << 8) | *(buffer++);
			/* no break */
		case 0x81:
			if ((length--) == 0) return -1;
			tagLength = (tagLength << 8) | *(buffer++);
			break;
		default:
			if (tmp >= 0x80) {
				// Other 8x variants are NOT supported
				return -1;
			}
			tagLength = tmp;
			break;
		}

		// Check value
		if (tagLength>length) return -1;

		// Process tag
		if (tagCallback(ctx, tag, tagLength, buffer)<0) {
			return -1;
		}
		buffer += tagLength;
		length -= tagLength;
	}
	return 0;
}