// FLOREA LARISA-ELENA 313CA

#include "image_editor.h"

int main(void)
{
	image *p, *aux_p; // p = photo
	select *s; // s = section
	int exit = 1, ok = 1, ph_loaded = 0, L;
	char command[40], file_name[40], word[40];
	p = malloc(1 * sizeof(image));
	aux_p = malloc(1 * sizeof(image));
	s = malloc(1 * sizeof(select));

	while (exit) {
		if (ok == 1)
			scanf("%s", command);
		else
			ok = 1;

		switch (convert(command)) {
		case 1: // LOAD
			scanf("%s", file_name);
			if (ph_loaded == 1)
				free_photo(p), free_photo(aux_p);
			L = LOAD(p, file_name);
			CHECK_LOAD(L, p, aux_p, s, &ph_loaded);
			break;
		case 2: // SELECT
			SELECT(p, s, ph_loaded); break;
		case 3: // HISTOGRAM
			HISTOGRAM(p, ph_loaded); break;
		case 4: // EQUALIZE
			EQUALIZE(p, ph_loaded); break;
		case 5: // ROTATE
			ROTATE(p, aux_p, s, ph_loaded); break;
		case 6: // CROP
			CROP(p, aux_p, s, ph_loaded); break;
		case 7: // APPLY
			scanf("%s", word);
			if (ph_loaded == 0) {
				printf("No image loaded\n");
				if (convert(word) != -1)
					ok = 0, strcpy(command, word);
				else
					getchar();
			} else {
				if (convert(word) != -1)
					printf("Invalid command\n"), ok = 0,
					strcpy(command, word);
				else
					APPLY(p, aux_p, s, word);
			}
			break;
		case 8: //SAVE
			scanf("%s %s", file_name, word);
			if (ph_loaded == 0) {
				printf("No image loaded\n");
				if (strcmp(word, "ascii") != 0)
					ok = 0, strcpy(command, word);
				else
					getchar();
			} else {
				if (strcmp(word, "ascii") == 0)
					SAVE_T(p, file_name);
				else
					ok = 0, strcpy(command, word),
					SAVE_B(p, file_name);
			}
			break;
		case 9: // EXIT
			if (ph_loaded == 0)
				printf("No image loaded\n");
			else
				free_photo(p), free_photo(aux_p);
			free(p); free(aux_p); free(s);
			exit = 0;
			getchar(); break;
		default: // INVALID COMMAND
			printf("Invalid command\n");
			fgets(word, sizeof(word), stdin); break;
		}
	}
}
