ldr r0, =0x20200004                                                             
ldr r1, =0x00001000                                                             
ldr r3, [r0]                                                                    
orr r3, r3, #0x1000000                                                          
and r3, r3, #0xfe000000                                                         
str r3, [r0]                                                                    
mov r4,#0x3F0000                                                                
wait :                                                                          
sub r4,r4,#1                                                                    
cmp r4,#0xFF                                                                    
bne wait                                                                        
str r1, [r0, #0x24]                                                             
str r1, [r0, #0x18]                                                             
mov r4, #0x3F0000                                                               
wait_again:                                                                     
sub r4, r4, #1                                                                  
cmp r4, #0xFF                                                                   
bne wait_again                                                                  
ldr r1, [r0, #0x24]                                                             
b wait              
