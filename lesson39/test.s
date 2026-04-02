
testthread:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 e1 2f 00 00 	mov    0x2fe1(%rip),%rax        # 3ff0 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 6a 2f 00 00    	push   0x2f6a(%rip)        # 3f90 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 6c 2f 00 00    	jmp    *0x2f6c(%rip)        # 3f98 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)
    1030:	f3 0f 1e fa          	endbr64
    1034:	68 00 00 00 00       	push   $0x0
    1039:	e9 e2 ff ff ff       	jmp    1020 <_init+0x20>
    103e:	66 90                	xchg   %ax,%ax
    1040:	f3 0f 1e fa          	endbr64
    1044:	68 01 00 00 00       	push   $0x1
    1049:	e9 d2 ff ff ff       	jmp    1020 <_init+0x20>
    104e:	66 90                	xchg   %ax,%ax
    1050:	f3 0f 1e fa          	endbr64
    1054:	68 02 00 00 00       	push   $0x2
    1059:	e9 c2 ff ff ff       	jmp    1020 <_init+0x20>
    105e:	66 90                	xchg   %ax,%ax
    1060:	f3 0f 1e fa          	endbr64
    1064:	68 03 00 00 00       	push   $0x3
    1069:	e9 b2 ff ff ff       	jmp    1020 <_init+0x20>
    106e:	66 90                	xchg   %ax,%ax
    1070:	f3 0f 1e fa          	endbr64
    1074:	68 04 00 00 00       	push   $0x4
    1079:	e9 a2 ff ff ff       	jmp    1020 <_init+0x20>
    107e:	66 90                	xchg   %ax,%ax
    1080:	f3 0f 1e fa          	endbr64
    1084:	68 05 00 00 00       	push   $0x5
    1089:	e9 92 ff ff ff       	jmp    1020 <_init+0x20>
    108e:	66 90                	xchg   %ax,%ax

Disassembly of section .plt.got:

0000000000001090 <__cxa_finalize@plt>:
    1090:	f3 0f 1e fa          	endbr64
    1094:	ff 25 36 2f 00 00    	jmp    *0x2f36(%rip)        # 3fd0 <__cxa_finalize@GLIBC_2.2.5>
    109a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

00000000000010a0 <sleep@plt>:
    10a0:	f3 0f 1e fa          	endbr64
    10a4:	ff 25 f6 2e 00 00    	jmp    *0x2ef6(%rip)        # 3fa0 <sleep@GLIBC_2.2.5>
    10aa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000010b0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>:
    10b0:	f3 0f 1e fa          	endbr64
    10b4:	ff 25 ee 2e 00 00    	jmp    *0x2eee(%rip)        # 3fa8 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@GLIBCXX_3.4>
    10ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000010c0 <_ZNSolsEPFRSoS_E@plt>:
    10c0:	f3 0f 1e fa          	endbr64
    10c4:	ff 25 e6 2e 00 00    	jmp    *0x2ee6(%rip)        # 3fb0 <_ZNSolsEPFRSoS_E@GLIBCXX_3.4>
    10ca:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000010d0 <getpid@plt>:
    10d0:	f3 0f 1e fa          	endbr64
    10d4:	ff 25 de 2e 00 00    	jmp    *0x2ede(%rip)        # 3fb8 <getpid@GLIBC_2.2.5>
    10da:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000010e0 <_ZNSolsEi@plt>:
    10e0:	f3 0f 1e fa          	endbr64
    10e4:	ff 25 d6 2e 00 00    	jmp    *0x2ed6(%rip)        # 3fc0 <_ZNSolsEi@GLIBCXX_3.4>
    10ea:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000010f0 <pthread_create@plt>:
    10f0:	f3 0f 1e fa          	endbr64
    10f4:	ff 25 ce 2e 00 00    	jmp    *0x2ece(%rip)        # 3fc8 <pthread_create@GLIBC_2.34>
    10fa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

Disassembly of section .text:

0000000000001100 <_start>:
    1100:	f3 0f 1e fa          	endbr64
    1104:	31 ed                	xor    %ebp,%ebp
    1106:	49 89 d1             	mov    %rdx,%r9
    1109:	5e                   	pop    %rsi
    110a:	48 89 e2             	mov    %rsp,%rdx
    110d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1111:	50                   	push   %rax
    1112:	54                   	push   %rsp
    1113:	45 31 c0             	xor    %r8d,%r8d
    1116:	31 c9                	xor    %ecx,%ecx
    1118:	48 8d 3d 54 01 00 00 	lea    0x154(%rip),%rdi        # 1273 <main>
    111f:	ff 15 bb 2e 00 00    	call   *0x2ebb(%rip)        # 3fe0 <__libc_start_main@GLIBC_2.34>
    1125:	f4                   	hlt
    1126:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    112d:	00 00 00 

0000000000001130 <deregister_tm_clones>:
    1130:	48 8d 3d d9 2e 00 00 	lea    0x2ed9(%rip),%rdi        # 4010 <__TMC_END__>
    1137:	48 8d 05 d2 2e 00 00 	lea    0x2ed2(%rip),%rax        # 4010 <__TMC_END__>
    113e:	48 39 f8             	cmp    %rdi,%rax
    1141:	74 15                	je     1158 <deregister_tm_clones+0x28>
    1143:	48 8b 05 9e 2e 00 00 	mov    0x2e9e(%rip),%rax        # 3fe8 <_ITM_deregisterTMCloneTable@Base>
    114a:	48 85 c0             	test   %rax,%rax
    114d:	74 09                	je     1158 <deregister_tm_clones+0x28>
    114f:	ff e0                	jmp    *%rax
    1151:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1158:	c3                   	ret
    1159:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001160 <register_tm_clones>:
    1160:	48 8d 3d a9 2e 00 00 	lea    0x2ea9(%rip),%rdi        # 4010 <__TMC_END__>
    1167:	48 8d 35 a2 2e 00 00 	lea    0x2ea2(%rip),%rsi        # 4010 <__TMC_END__>
    116e:	48 29 fe             	sub    %rdi,%rsi
    1171:	48 89 f0             	mov    %rsi,%rax
    1174:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1178:	48 c1 f8 03          	sar    $0x3,%rax
    117c:	48 01 c6             	add    %rax,%rsi
    117f:	48 d1 fe             	sar    $1,%rsi
    1182:	74 14                	je     1198 <register_tm_clones+0x38>
    1184:	48 8b 05 6d 2e 00 00 	mov    0x2e6d(%rip),%rax        # 3ff8 <_ITM_registerTMCloneTable@Base>
    118b:	48 85 c0             	test   %rax,%rax
    118e:	74 08                	je     1198 <register_tm_clones+0x38>
    1190:	ff e0                	jmp    *%rax
    1192:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1198:	c3                   	ret
    1199:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000011a0 <__do_global_dtors_aux>:
    11a0:	f3 0f 1e fa          	endbr64
    11a4:	80 3d a5 2f 00 00 00 	cmpb   $0x0,0x2fa5(%rip)        # 4150 <completed.0>
    11ab:	75 2b                	jne    11d8 <__do_global_dtors_aux+0x38>
    11ad:	55                   	push   %rbp
    11ae:	48 83 3d 1a 2e 00 00 	cmpq   $0x0,0x2e1a(%rip)        # 3fd0 <__cxa_finalize@GLIBC_2.2.5>
    11b5:	00 
    11b6:	48 89 e5             	mov    %rsp,%rbp
    11b9:	74 0c                	je     11c7 <__do_global_dtors_aux+0x27>
    11bb:	48 8b 3d 46 2e 00 00 	mov    0x2e46(%rip),%rdi        # 4008 <__dso_handle>
    11c2:	e8 c9 fe ff ff       	call   1090 <__cxa_finalize@plt>
    11c7:	e8 64 ff ff ff       	call   1130 <deregister_tm_clones>
    11cc:	c6 05 7d 2f 00 00 01 	movb   $0x1,0x2f7d(%rip)        # 4150 <completed.0>
    11d3:	5d                   	pop    %rbp
    11d4:	c3                   	ret
    11d5:	0f 1f 00             	nopl   (%rax)
    11d8:	c3                   	ret
    11d9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000011e0 <frame_dummy>:
    11e0:	f3 0f 1e fa          	endbr64
    11e4:	e9 77 ff ff ff       	jmp    1160 <register_tm_clones>

00000000000011e9 <_Z5helloPv>:
    11e9:	f3 0f 1e fa          	endbr64
    11ed:	55                   	push   %rbp
    11ee:	48 89 e5             	mov    %rsp,%rbp
    11f1:	53                   	push   %rbx
    11f2:	48 83 ec 28          	sub    $0x28,%rsp
    11f6:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    11fa:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    11fe:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    1202:	48 8d 05 fb 0d 00 00 	lea    0xdfb(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    1209:	48 89 c6             	mov    %rax,%rsi
    120c:	48 8d 05 2d 2e 00 00 	lea    0x2e2d(%rip),%rax        # 4040 <_ZSt4cout@GLIBCXX_3.4>
    1213:	48 89 c7             	mov    %rax,%rdi
    1216:	e8 95 fe ff ff       	call   10b0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
    121b:	48 89 c3             	mov    %rax,%rbx
    121e:	e8 ad fe ff ff       	call   10d0 <getpid@plt>
    1223:	89 c6                	mov    %eax,%esi
    1225:	48 89 df             	mov    %rbx,%rdi
    1228:	e8 b3 fe ff ff       	call   10e0 <_ZNSolsEi@plt>
    122d:	48 89 c2             	mov    %rax,%rdx
    1230:	48 8d 05 e6 0d 00 00 	lea    0xde6(%rip),%rax        # 201d <_IO_stdin_used+0x1d>
    1237:	48 89 c6             	mov    %rax,%rsi
    123a:	48 89 d7             	mov    %rdx,%rdi
    123d:	e8 6e fe ff ff       	call   10b0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
    1242:	48 89 c2             	mov    %rax,%rdx
    1245:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    1249:	48 89 c6             	mov    %rax,%rsi
    124c:	48 89 d7             	mov    %rdx,%rdi
    124f:	e8 5c fe ff ff       	call   10b0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
    1254:	48 8b 15 7d 2d 00 00 	mov    0x2d7d(%rip),%rdx        # 3fd8 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
    125b:	48 89 d6             	mov    %rdx,%rsi
    125e:	48 89 c7             	mov    %rax,%rdi
    1261:	e8 5a fe ff ff       	call   10c0 <_ZNSolsEPFRSoS_E@plt>
    1266:	bf 01 00 00 00       	mov    $0x1,%edi
    126b:	e8 30 fe ff ff       	call   10a0 <sleep@plt>
    1270:	90                   	nop
    1271:	eb 8f                	jmp    1202 <_Z5helloPv+0x19>

0000000000001273 <main>:
    1273:	f3 0f 1e fa          	endbr64
    1277:	55                   	push   %rbp
    1278:	48 89 e5             	mov    %rsp,%rbp
    127b:	53                   	push   %rbx
    127c:	48 83 ec 18          	sub    $0x18,%rsp
    1280:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1287:	00 00 
    1289:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    128d:	31 c0                	xor    %eax,%eax
    128f:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
    1293:	48 8d 15 8e 0d 00 00 	lea    0xd8e(%rip),%rdx        # 2028 <_IO_stdin_used+0x28>
    129a:	48 89 d1             	mov    %rdx,%rcx
    129d:	48 8d 15 45 ff ff ff 	lea    -0xbb(%rip),%rdx        # 11e9 <_Z5helloPv>
    12a4:	be 00 00 00 00       	mov    $0x0,%esi
    12a9:	48 89 c7             	mov    %rax,%rdi
    12ac:	e8 3f fe ff ff       	call   10f0 <pthread_create@plt>
    12b1:	48 8d 05 7b 0d 00 00 	lea    0xd7b(%rip),%rax        # 2033 <_IO_stdin_used+0x33>
    12b8:	48 89 c6             	mov    %rax,%rsi
    12bb:	48 8d 05 7e 2d 00 00 	lea    0x2d7e(%rip),%rax        # 4040 <_ZSt4cout@GLIBCXX_3.4>
    12c2:	48 89 c7             	mov    %rax,%rdi
    12c5:	e8 e6 fd ff ff       	call   10b0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
    12ca:	48 89 c3             	mov    %rax,%rbx
    12cd:	e8 fe fd ff ff       	call   10d0 <getpid@plt>
    12d2:	89 c6                	mov    %eax,%esi
    12d4:	48 89 df             	mov    %rbx,%rdi
    12d7:	e8 04 fe ff ff       	call   10e0 <_ZNSolsEi@plt>
    12dc:	48 8b 15 f5 2c 00 00 	mov    0x2cf5(%rip),%rdx        # 3fd8 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
    12e3:	48 89 d6             	mov    %rdx,%rsi
    12e6:	48 89 c7             	mov    %rax,%rdi
    12e9:	e8 d2 fd ff ff       	call   10c0 <_ZNSolsEPFRSoS_E@plt>
    12ee:	bf 01 00 00 00       	mov    $0x1,%edi
    12f3:	e8 a8 fd ff ff       	call   10a0 <sleep@plt>
    12f8:	eb b7                	jmp    12b1 <main+0x3e>

Disassembly of section .fini:

00000000000012fc <_fini>:
    12fc:	f3 0f 1e fa          	endbr64
    1300:	48 83 ec 08          	sub    $0x8,%rsp
    1304:	48 83 c4 08          	add    $0x8,%rsp
    1308:	c3                   	ret
