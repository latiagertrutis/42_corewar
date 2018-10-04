.name "bigmom"
.comment "I will kill you if I have to"

set_nb:     sti r1, %:live, %1
            and r2, %0, r3

live:       live %1

fork:       fork %:set_nb

set_nb2:     sti r1, %:live2, %1
live2:      live %1

set_nb3:    sti r1, %:live3, %1
live3:		live %1

set_nb4:    sti r1, %:live4, %1
live4:		live %1

set_nb5:    sti r1, %:live5, %1
live5:		live %1

set_nb6:    sti r1, %:live6, %1
live6:		live %1

set_nb7:    sti r1, %:live7, %1
live7:		live %1

set_nb8:    sti r1, %:live8, %1
live8:		live %1

set_nb9:    sti r1, %:live9, %1
live9:		live %1

set_nb10:	sti r1, %:live10, %1
live10:		live %1

set_nb11:	sti r1, %:live11, %1
live11:		live %1

set_nb15:	sti r1, %:live15, %1
live15:		live %1

set_nb13:	sti r1, %:live13, %1
live13:		live %1

set_nb14:	sti r1, %:live14, %1
live14:		live %1
