#define CONCAT_0_(prefix, T0) prefix##_##T0
#define CONCAT_0(prefix, T0) CONCAT_0_(prefix, T0)

#define CONCAT_1_(prefix, T0, T1) prefix##_##T0##_T1
#define CONCAT_1(prefix, T0, T1) CONCAT_1_(prefix, T0, T1)

#define CONCAT_2_(prefix, T0, T1, T2) prefix##_##T0##_T1##_T2
#define CONCAT_2(prefix, T0, T1, T2) CONCAT_2_(prefix, T0, T1, T2)

#define CONCAT_3_(prefix, T0, T1, T2, T3) prefix##_##T0##_T1##_T2##_T3
#define CONCAT_3(prefix, T0, T1, T2, T3) CONCAT_3_(prefix, T0, T1, T2, T3)

#define CONCAT_4_(prefix, T0, T1, T2, T3, T4) prefix##_##T0##_T1##_T2##_T3##_T4
#define CONCAT_4(prefix, T0, T1, T2, T3, T4) CONCAT_4_(prefix, T0, T1, T2, T3, T4)

#define CONCAT_5_(prefix, T0, T1, T2, T3, T4, T5) prefix##_##T0##_T1##_T2##_T3##_T4##_T5
#define CONCAT_5(prefix, T0, T1, T2, T3, T4, T5) CONCAT_5_(prefix, T0, T1, T2, T3, T4, T5)

#define CONCAT_6_(prefix, T0, T1, T2, T3, T4, T5, T6) prefix##_##T0##_T1##_T2##_T3##_T4##_T5##_T6
#define CONCAT_6(prefix, T0, T1, T2, T3, T4, T5, T6) CONCAT_6_(prefix, T0, T1, T2, T3, T4, T5, T6)

#define CONCAT_7_(prefix, T0, T1, T2, T3, T4, T5, T6, T7) prefix##_##T0##_T1##_T2##_T3##_T4##_T5##_T6##_T7
#define CONCAT_7(prefix, T0, T1, T2, T3, T4, T5, T6, T7) CONCAT_7_(prefix, T0, T1, T2, T3, T4, T5, T6, T7)

#define CONCAT_8_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8) prefix##_##T0##_T1##_T2##_T3##_T4##_T5##_T6##_T7##_T8
#define CONCAT_8(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8) CONCAT_8_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8)

#define CONCAT_9_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) prefix##_##T0##_T1##_T2##_T3##_T4##_T5##_T6##_T7##_T8##_T9
#define CONCAT_9(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) CONCAT_9_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)

