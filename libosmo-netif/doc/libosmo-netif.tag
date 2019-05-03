<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>jibuf.h</name>
    <path>/mnt/sda/sources/osmo/libosmo-netif/include/osmocom/netif/</path>
    <filename>jibuf_8h</filename>
    <class kind="struct">osmo_jibuf</class>
    <member kind="function">
      <type>struct osmo_jibuf *</type>
      <name>osmo_jibuf_alloc</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga20592f230f9d1233752fd318996f2c9d</anchor>
      <arglist>(void *talloc_ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_delete</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gacdac7beaf3d97543718ddfa33123fb7e</anchor>
      <arglist>(struct osmo_jibuf *jb)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_jibuf_enqueue</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga522165ddc51578436ae2d26691fa7ec0</anchor>
      <arglist>(struct osmo_jibuf *jb, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>osmo_jibuf_empty</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gae73a3b805b83f453339adeee9cdfaba4</anchor>
      <arglist>(struct osmo_jibuf *jb)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_min_delay</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gabcd63f39049bc4d6b3541a674b78963d</anchor>
      <arglist>(struct osmo_jibuf *jb, uint32_t min_delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_max_delay</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga5e84571c3348525b6ad647a22cd2581d</anchor>
      <arglist>(struct osmo_jibuf *jb, uint32_t max_delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_enable_skew_compensation</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga096e87b2dbdade72c35d51a51fe57839</anchor>
      <arglist>(struct osmo_jibuf *jb, bool enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_dequeue_cb</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga3a8213fe8bed42d649fcb1c8d13e23d8</anchor>
      <arglist>(struct osmo_jibuf *jb, osmo_jibuf_dequeue_cb dequeue_cb, void *cb_data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>osmux.h</name>
    <path>/mnt/sda/sources/osmo/libosmo-netif/include/osmocom/netif/</path>
    <filename>osmux_8h</filename>
    <class kind="struct">osmux_hdr</class>
    <class kind="struct">osmux_in_handle</class>
    <class kind="struct">osmux_out_handle</class>
    <member kind="function">
      <type>int</type>
      <name>osmux_snprintf</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gaf654e092ca97dd764f59905a2ed21650</anchor>
      <arglist>(char *buf, size_t size, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmux_xfrm_input</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>ga9c7d30c0d7d9bc0caf744799d629d4cc</anchor>
      <arglist>(struct osmux_in_handle *h, struct msgb *msg, int ccid)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmux_xfrm_output_set_tx_cb</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>ga3936e43e881d0992648937417969ead1</anchor>
      <arglist>(struct osmux_out_handle *h, void(*tx_cb)(struct msgb *msg, void *data), void *data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmux_xfrm_output_sched</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gafee78ee4522a9caa238301d7b913dfd6</anchor>
      <arglist>(struct osmux_out_handle *h, struct osmux_hdr *osmuxh)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmux_xfrm_output_flush</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gaaac410425f6470396ae9db53ebb8dc44</anchor>
      <arglist>(struct osmux_out_handle *h)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>datagram.c</name>
    <path>/mnt/sda/sources/osmo/libosmo-netif/src/</path>
    <filename>datagram_8c</filename>
    <class kind="struct">osmo_dgram_tx</class>
    <class kind="struct">osmo_dgram_rx</class>
    <class kind="struct">osmo_dgram</class>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_close</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga0eb580467681e8fa91be35a83a74f132</anchor>
      <arglist>(struct osmo_dgram_tx *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_dgram_tx *</type>
      <name>osmo_dgram_tx_create</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga946c84a9ab38f90c3589ddccf9df676e</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga94fe94f64f1285715db36eb1f2e9deaa</anchor>
      <arglist>(struct osmo_dgram_tx *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga48d7e39656886b52aa176fc1b14c84cf</anchor>
      <arglist>(struct osmo_dgram_tx *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_local_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6c843f43db5439f8f0ba38f72dbb1a20</anchor>
      <arglist>(struct osmo_dgram_tx *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_local_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gae2962232548d41621520c6c71da423a5</anchor>
      <arglist>(struct osmo_dgram_tx *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_data</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga095ce94e65f1607026c37c12e6f5e9c0</anchor>
      <arglist>(struct osmo_dgram_tx *conn, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_destroy</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaad6f9ca0688cd4a8a95ec6002f52ded5</anchor>
      <arglist>(struct osmo_dgram_tx *conn)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_tx_open</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga8d712623df3862e55ad248dc5ecd626c</anchor>
      <arglist>(struct osmo_dgram_tx *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_send</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gad4638b9943090f5bf1a7b75fa8efde46</anchor>
      <arglist>(struct osmo_dgram_tx *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_rx_recv</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga16d69e998c663cdec0b76560a2d8cfb9</anchor>
      <arglist>(struct osmo_dgram_rx *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_dgram_rx *</type>
      <name>osmo_dgram_rx_create</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga3e9ef6bd02ce0b9bc42ae57ca2e5fb71</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_set_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gad378ba5cc66ee0c27a277906d8434e31</anchor>
      <arglist>(struct osmo_dgram_rx *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_set_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gafbd947def18d8e34062876433ac05e91</anchor>
      <arglist>(struct osmo_dgram_rx *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_set_read_cb</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga5a0f7221b8583aac5153f167c4e2177d</anchor>
      <arglist>(struct osmo_dgram_rx *conn, int(*read_cb)(struct osmo_dgram_rx *conn))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_destroy</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6e4b92b9c7f210d1cb3782425d0bdac0</anchor>
      <arglist>(struct osmo_dgram_rx *conn)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_rx_open</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaf558e9c2f02216ed95db039ddaf31050</anchor>
      <arglist>(struct osmo_dgram_rx *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_close</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga01efa7269834a745d80910a64c6f61c0</anchor>
      <arglist>(struct osmo_dgram_rx *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_dgram *</type>
      <name>osmo_dgram_create</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga4efd892e0ddb3551d9330e091b65056b</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_destroy</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga01fb36f8c19b2b64705d8263f7f53f59</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_local_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaf21d974e14fa22f910313b240d31d40f</anchor>
      <arglist>(struct osmo_dgram *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_remote_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6677ee8d976d2e1bed8e0c01e103c18f</anchor>
      <arglist>(struct osmo_dgram *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_local_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga9180ae9a36a9927d4bdb49cdc5708b1f</anchor>
      <arglist>(struct osmo_dgram *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_remote_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga27bf4c1d8e4c30ef7b4ec49665d6fc90</anchor>
      <arglist>(struct osmo_dgram *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_read_cb</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6b6ef2f02e3d260f1e84a2ecf8c67b5a</anchor>
      <arglist>(struct osmo_dgram *conn, int(*read_cb)(struct osmo_dgram *conn))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_data</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaa6a5fbd5f227d15cefe6b234fefb3b4d</anchor>
      <arglist>(struct osmo_dgram *conn, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_dgram_get_data</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gad2cbf018a7fce486b2cd5cc174b24974</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_open</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gac65de108c52dba83b178d6453e9fb545</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_close</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga46db4acbfc35668e35601338d64364ed</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_send</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga66d6c9d5f0243a4f7ed458c62d8e20dd</anchor>
      <arglist>(struct osmo_dgram *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_recv</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaae58d13a6fd634ebfddd486d0d428c5e</anchor>
      <arglist>(struct osmo_dgram *conn, struct msgb *msg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jibuf.c</name>
    <path>/mnt/sda/sources/osmo/libosmo-netif/src/</path>
    <filename>jibuf_8c</filename>
    <includes id="jibuf_8h" name="jibuf.h" local="no" imported="no">osmocom/netif/jibuf.h</includes>
    <class kind="struct">osmo_jibuf_msgb_cb</class>
    <member kind="function">
      <type>struct osmo_jibuf *</type>
      <name>osmo_jibuf_alloc</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga20592f230f9d1233752fd318996f2c9d</anchor>
      <arglist>(void *talloc_ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_delete</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gacdac7beaf3d97543718ddfa33123fb7e</anchor>
      <arglist>(struct osmo_jibuf *jb)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_jibuf_enqueue</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga522165ddc51578436ae2d26691fa7ec0</anchor>
      <arglist>(struct osmo_jibuf *jb, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>osmo_jibuf_empty</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gae73a3b805b83f453339adeee9cdfaba4</anchor>
      <arglist>(struct osmo_jibuf *jb)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_min_delay</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gabcd63f39049bc4d6b3541a674b78963d</anchor>
      <arglist>(struct osmo_jibuf *jb, uint32_t min_delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_max_delay</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga5e84571c3348525b6ad647a22cd2581d</anchor>
      <arglist>(struct osmo_jibuf *jb, uint32_t max_delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_enable_skew_compensation</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga096e87b2dbdade72c35d51a51fe57839</anchor>
      <arglist>(struct osmo_jibuf *jb, bool enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_dequeue_cb</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga3a8213fe8bed42d649fcb1c8d13e23d8</anchor>
      <arglist>(struct osmo_jibuf *jb, osmo_jibuf_dequeue_cb dequeue_cb, void *cb_data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>osmux.c</name>
    <path>/mnt/sda/sources/osmo/libosmo-netif/src/</path>
    <filename>osmux_8c</filename>
    <includes id="osmux_8h" name="osmux.h" local="no" imported="no">osmocom/netif/osmux.h</includes>
    <class kind="struct">osmux_batch</class>
    <class kind="struct">osmux_circuit</class>
    <class kind="struct">osmux_input_state</class>
    <class kind="struct">osmux_tx_handle</class>
    <member kind="function">
      <type>int</type>
      <name>osmux_xfrm_output_sched</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gafee78ee4522a9caa238301d7b913dfd6</anchor>
      <arglist>(struct osmux_out_handle *h, struct osmux_hdr *osmuxh)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmux_xfrm_output_flush</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gaaac410425f6470396ae9db53ebb8dc44</anchor>
      <arglist>(struct osmux_out_handle *h)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmux_xfrm_input</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>ga9c7d30c0d7d9bc0caf744799d629d4cc</anchor>
      <arglist>(struct osmux_in_handle *h, struct msgb *msg, int ccid)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmux_xfrm_output_set_tx_cb</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>ga3936e43e881d0992648937417969ead1</anchor>
      <arglist>(struct osmux_out_handle *h, void(*tx_cb)(struct msgb *msg, void *data), void *data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmux_snprintf</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gaf654e092ca97dd764f59905a2ed21650</anchor>
      <arglist>(char *buf, size_t size, struct msgb *msg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stream.c</name>
    <path>/mnt/sda/sources/osmo/libosmo-netif/src/</path>
    <filename>stream_8c</filename>
    <class kind="struct">osmo_stream_cli</class>
    <class kind="struct">osmo_stream_srv_link</class>
    <class kind="struct">osmo_stream_srv</class>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_close</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gad217c1c1b83ac852e090afd49146db19</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_reconnect</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga11d2615f6197f33073ba7245e5ba95ee</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_cli *</type>
      <name>osmo_stream_cli_create</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga1754a498abc27577a8e0b3b0f236c05b</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_addr</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga0038e937cf0f36c59e0e3991523219a1</anchor>
      <arglist>(struct osmo_stream_cli *cli, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_port</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gab8b9f65bebfeb83c6e33d102e9439770</anchor>
      <arglist>(struct osmo_stream_cli *cli, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_local_port</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga4d83e3cf7fdf7eb354c2015ff9bed98a</anchor>
      <arglist>(struct osmo_stream_cli *cli, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_local_addr</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga70420e7c664cc4c4e69ac4d3476e7f53</anchor>
      <arglist>(struct osmo_stream_cli *cli, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_proto</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga91d57aa94a370520540d7272af66a23b</anchor>
      <arglist>(struct osmo_stream_cli *cli, uint16_t proto)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_reconnect_timeout</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gade74792afe35f31dd2584c888db3dd62</anchor>
      <arglist>(struct osmo_stream_cli *cli, int timeout)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga5e94c511d537f63337912bb50fb2dc28</anchor>
      <arglist>(struct osmo_stream_cli *cli, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_stream_cli_get_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga606a53840677f3f5200887dc92f5de4d</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>osmo_stream_cli_get_sockname</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaecd148ae11f2f619aa52dd2900a15827</anchor>
      <arglist>(const struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_fd *</type>
      <name>osmo_stream_cli_get_ofd</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga0615daa4aac253a3d530f35fddbc68bd</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_connect_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gab588bfd88a3cc775875981e2ee66e295</anchor>
      <arglist>(struct osmo_stream_cli *cli, int(*connect_cb)(struct osmo_stream_cli *cli))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_disconnect_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga23e5c1799a8225607543de39a56ca449</anchor>
      <arglist>(struct osmo_stream_cli *cli, int(*disconnect_cb)(struct osmo_stream_cli *cli))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_read_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gad1d1817f25694ea126779262cddf4fbd</anchor>
      <arglist>(struct osmo_stream_cli *cli, int(*read_cb)(struct osmo_stream_cli *cli))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga9d0dac2f390c660c85fa347765f4f45a</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_cli_open2</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga127866794591d16a0eb7244f2fe666ef</anchor>
      <arglist>(struct osmo_stream_cli *cli, int reconnect)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_nodelay</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaa196e78db6ca32abb1473566b4f1b04b</anchor>
      <arglist>(struct osmo_stream_cli *cli, bool nodelay)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_cli_open</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga86b4b9c8e563f8426d3cdae803e3ec38</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_send</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaacf415b97e1184baadeee5c60387422b</anchor>
      <arglist>(struct osmo_stream_cli *cli, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_cli_recv</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga06f9743c19068bb720a4949ae8467243</anchor>
      <arglist>(struct osmo_stream_cli *cli, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_srv_link *</type>
      <name>osmo_stream_srv_link_create</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaad262988c7ac365566f03843c560f892</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_nodelay</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga41e8ad2c761aaa00313aa11d1f29a3ed</anchor>
      <arglist>(struct osmo_stream_srv_link *link, bool nodelay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_addr</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gacece90116484e34540def69b05c66f57</anchor>
      <arglist>(struct osmo_stream_srv_link *link, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_port</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga76bbbafd1d8dc9c9ebffd301503b6dd9</anchor>
      <arglist>(struct osmo_stream_srv_link *link, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_proto</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga2508987133a93863d2a50103004b0f36</anchor>
      <arglist>(struct osmo_stream_srv_link *link, uint16_t proto)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga5ef0f0c85a52c08e8c10d36a8f44e392</anchor>
      <arglist>(struct osmo_stream_srv_link *link, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_stream_srv_link_get_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gacb5d68c2a6a9663f99266c70747bce04</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>osmo_stream_srv_link_get_sockname</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga7d7b6af7df24e90dc758a088bae6316d</anchor>
      <arglist>(const struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_fd *</type>
      <name>osmo_stream_srv_link_get_ofd</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga7b8981351a154c357fcefd39c31ad42a</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_accept_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga1eea525d4e1e7a24c9c08881994ceb7e</anchor>
      <arglist>(struct osmo_stream_srv_link *link, int(*accept_cb)(struct osmo_stream_srv_link *link, int fd))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga882acaaa5935422b0e09a2ce8c8019ec</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_srv_link_open</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga603b89074d3e8c0fb44144ab1864f0d4</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_close</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gad325d97b09c724e830c98d76b890520d</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_srv *</type>
      <name>osmo_stream_srv_create</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga36df4fc3b268efbe4b4f02e51542d543</anchor>
      <arglist>(void *ctx, struct osmo_stream_srv_link *link, int fd, int(*cb)(struct osmo_stream_srv *conn), int(*closed_cb)(struct osmo_stream_srv *conn), void *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_set_flush_and_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga8390ce6740e56eb22215e1c5805c95f1</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_set_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga9891663b20714d72f992e44ad1e92eea</anchor>
      <arglist>(struct osmo_stream_srv *conn, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_stream_srv_get_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gae2a0ef01097d964c71e38600b4c078d5</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_fd *</type>
      <name>osmo_stream_srv_get_ofd</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga47bbca4f12717db2e6158eefe6310b9b</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_srv_link *</type>
      <name>osmo_stream_srv_get_master</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga9cf689935bf6ffac4f86f56219d73c8e</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga47f18e063f64f22cb74d58d8b7243bed</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_send</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaa1589994cf6eab27742cafee6b7fb491</anchor>
      <arglist>(struct osmo_stream_srv *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_srv_recv</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gac86242c41223c525c770ff7e7515d83e</anchor>
      <arglist>(struct osmo_stream_srv *conn, struct msgb *msg)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>amr_hdr</name>
    <filename>structamr__hdr.html</filename>
  </compound>
  <compound kind="struct">
    <name>baudrate2termbits</name>
    <filename>structbaudrate2termbits.html</filename>
  </compound>
  <compound kind="struct">
    <name>ipa_head</name>
    <filename>structipa__head.html</filename>
  </compound>
  <compound kind="struct">
    <name>ipa_head_ext</name>
    <filename>structipa__head__ext.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_dgram</name>
    <filename>structosmo__dgram.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_dgram_rx</name>
    <filename>structosmo__dgram__rx.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_dgram_tx</name>
    <filename>structosmo__dgram__tx.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_ipa_unit</name>
    <filename>structosmo__ipa__unit.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_jibuf</name>
    <filename>structosmo__jibuf.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_jibuf_msgb_cb</name>
    <filename>structosmo__jibuf__msgb__cb.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_rs232</name>
    <filename>structosmo__rs232.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_rtp_handle</name>
    <filename>structosmo__rtp__handle.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_stream_cli</name>
    <filename>structosmo__stream__cli.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_stream_srv</name>
    <filename>structosmo__stream__srv.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmo_stream_srv_link</name>
    <filename>structosmo__stream__srv__link.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmux_batch</name>
    <filename>structosmux__batch.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmux_circuit</name>
    <filename>structosmux__circuit.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmux_hdr</name>
    <filename>structosmux__hdr.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmux_in_handle</name>
    <filename>structosmux__in__handle.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmux_input_state</name>
    <filename>structosmux__input__state.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmux_out_handle</name>
    <filename>structosmux__out__handle.html</filename>
  </compound>
  <compound kind="struct">
    <name>osmux_tx_handle</name>
    <filename>structosmux__tx__handle.html</filename>
  </compound>
  <compound kind="struct">
    <name>rtcp_hdr</name>
    <filename>structrtcp__hdr.html</filename>
  </compound>
  <compound kind="struct">
    <name>rtp_hdr</name>
    <filename>structrtp__hdr.html</filename>
  </compound>
  <compound kind="struct">
    <name>rtp_x_hdr</name>
    <filename>structrtp__x__hdr.html</filename>
  </compound>
  <compound kind="group">
    <name>jibuf</name>
    <title>Osmocom Jitter Buffer</title>
    <filename>group__jibuf.html</filename>
    <file>jibuf.h</file>
    <file>jibuf.c</file>
    <class kind="struct">osmo_jibuf</class>
    <class kind="struct">osmo_jibuf_msgb_cb</class>
    <member kind="function">
      <type>struct osmo_jibuf *</type>
      <name>osmo_jibuf_alloc</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga20592f230f9d1233752fd318996f2c9d</anchor>
      <arglist>(void *talloc_ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_delete</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gacdac7beaf3d97543718ddfa33123fb7e</anchor>
      <arglist>(struct osmo_jibuf *jb)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_jibuf_enqueue</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga522165ddc51578436ae2d26691fa7ec0</anchor>
      <arglist>(struct osmo_jibuf *jb, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>osmo_jibuf_empty</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gae73a3b805b83f453339adeee9cdfaba4</anchor>
      <arglist>(struct osmo_jibuf *jb)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_min_delay</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>gabcd63f39049bc4d6b3541a674b78963d</anchor>
      <arglist>(struct osmo_jibuf *jb, uint32_t min_delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_max_delay</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga5e84571c3348525b6ad647a22cd2581d</anchor>
      <arglist>(struct osmo_jibuf *jb, uint32_t max_delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_enable_skew_compensation</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga096e87b2dbdade72c35d51a51fe57839</anchor>
      <arglist>(struct osmo_jibuf *jb, bool enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_jibuf_set_dequeue_cb</name>
      <anchorfile>group__jibuf.html</anchorfile>
      <anchor>ga3a8213fe8bed42d649fcb1c8d13e23d8</anchor>
      <arglist>(struct osmo_jibuf *jb, osmo_jibuf_dequeue_cb dequeue_cb, void *cb_data)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>osmux</name>
    <title>Osmocom Multiplex Protocol</title>
    <filename>group__osmux.html</filename>
    <file>osmux.h</file>
    <file>osmux.c</file>
    <class kind="struct">osmux_hdr</class>
    <class kind="struct">osmux_in_handle</class>
    <class kind="struct">osmux_out_handle</class>
    <class kind="struct">osmux_batch</class>
    <class kind="struct">osmux_circuit</class>
    <class kind="struct">osmux_input_state</class>
    <class kind="struct">osmux_tx_handle</class>
    <member kind="function">
      <type>int</type>
      <name>osmux_snprintf</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gaf654e092ca97dd764f59905a2ed21650</anchor>
      <arglist>(char *buf, size_t size, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmux_xfrm_input</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>ga9c7d30c0d7d9bc0caf744799d629d4cc</anchor>
      <arglist>(struct osmux_in_handle *h, struct msgb *msg, int ccid)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmux_xfrm_output_set_tx_cb</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>ga3936e43e881d0992648937417969ead1</anchor>
      <arglist>(struct osmux_out_handle *h, void(*tx_cb)(struct msgb *msg, void *data), void *data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmux_xfrm_output_sched</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gafee78ee4522a9caa238301d7b913dfd6</anchor>
      <arglist>(struct osmux_out_handle *h, struct osmux_hdr *osmuxh)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmux_xfrm_output_flush</name>
      <anchorfile>group__osmux.html</anchorfile>
      <anchor>gaaac410425f6470396ae9db53ebb8dc44</anchor>
      <arglist>(struct osmux_out_handle *h)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>stream</name>
    <title>Osmocom Stream Socket</title>
    <filename>group__stream.html</filename>
    <file>stream.c</file>
    <class kind="struct">osmo_stream_cli</class>
    <class kind="struct">osmo_stream_srv_link</class>
    <class kind="struct">osmo_stream_srv</class>
    <member kind="define">
      <type>#define</type>
      <name>msgb_sctp_ppid</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga2693c0cc5c6222dd9ed42c8c4bf8aec8</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>msgb_sctp_stream</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gac2c359892ad63309b59a4b402f6a3e81</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_srv_link *</type>
      <name>osmo_stream_srv_link_create</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaad262988c7ac365566f03843c560f892</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga882acaaa5935422b0e09a2ce8c8019ec</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_nodelay</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga41e8ad2c761aaa00313aa11d1f29a3ed</anchor>
      <arglist>(struct osmo_stream_srv_link *link, bool nodelay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_addr</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gacece90116484e34540def69b05c66f57</anchor>
      <arglist>(struct osmo_stream_srv_link *link, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_port</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga76bbbafd1d8dc9c9ebffd301503b6dd9</anchor>
      <arglist>(struct osmo_stream_srv_link *link, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_proto</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga2508987133a93863d2a50103004b0f36</anchor>
      <arglist>(struct osmo_stream_srv_link *link, uint16_t proto)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_accept_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga1eea525d4e1e7a24c9c08881994ceb7e</anchor>
      <arglist>(struct osmo_stream_srv_link *link, int(*accept_cb)(struct osmo_stream_srv_link *link, int fd))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_set_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga5ef0f0c85a52c08e8c10d36a8f44e392</anchor>
      <arglist>(struct osmo_stream_srv_link *link, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_stream_srv_link_get_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gacb5d68c2a6a9663f99266c70747bce04</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>osmo_stream_srv_link_get_sockname</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga7d7b6af7df24e90dc758a088bae6316d</anchor>
      <arglist>(const struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_fd *</type>
      <name>osmo_stream_srv_link_get_ofd</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga7b8981351a154c357fcefd39c31ad42a</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_srv_link_open</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga603b89074d3e8c0fb44144ab1864f0d4</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_link_close</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gad325d97b09c724e830c98d76b890520d</anchor>
      <arglist>(struct osmo_stream_srv_link *link)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_srv *</type>
      <name>osmo_stream_srv_create</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga36df4fc3b268efbe4b4f02e51542d543</anchor>
      <arglist>(void *ctx, struct osmo_stream_srv_link *link, int fd, int(*cb)(struct osmo_stream_srv *conn), int(*closed_cb)(struct osmo_stream_srv *conn), void *data)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_stream_srv_get_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gae2a0ef01097d964c71e38600b4c078d5</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_srv_link *</type>
      <name>osmo_stream_srv_get_master</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga9cf689935bf6ffac4f86f56219d73c8e</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_fd *</type>
      <name>osmo_stream_srv_get_ofd</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga47bbca4f12717db2e6158eefe6310b9b</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga47f18e063f64f22cb74d58d8b7243bed</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_set_flush_and_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga8390ce6740e56eb22215e1c5805c95f1</anchor>
      <arglist>(struct osmo_stream_srv *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_set_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga9891663b20714d72f992e44ad1e92eea</anchor>
      <arglist>(struct osmo_stream_srv *conn, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_srv_send</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaa1589994cf6eab27742cafee6b7fb491</anchor>
      <arglist>(struct osmo_stream_srv *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_srv_recv</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gac86242c41223c525c770ff7e7515d83e</anchor>
      <arglist>(struct osmo_stream_srv *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_nodelay</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaa196e78db6ca32abb1473566b4f1b04b</anchor>
      <arglist>(struct osmo_stream_cli *cli, bool nodelay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_addr</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga0038e937cf0f36c59e0e3991523219a1</anchor>
      <arglist>(struct osmo_stream_cli *cli, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_port</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gab8b9f65bebfeb83c6e33d102e9439770</anchor>
      <arglist>(struct osmo_stream_cli *cli, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_proto</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga91d57aa94a370520540d7272af66a23b</anchor>
      <arglist>(struct osmo_stream_cli *cli, uint16_t proto)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_local_addr</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga70420e7c664cc4c4e69ac4d3476e7f53</anchor>
      <arglist>(struct osmo_stream_cli *cli, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_local_port</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga4d83e3cf7fdf7eb354c2015ff9bed98a</anchor>
      <arglist>(struct osmo_stream_cli *cli, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga5e94c511d537f63337912bb50fb2dc28</anchor>
      <arglist>(struct osmo_stream_cli *cli, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_reconnect_timeout</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gade74792afe35f31dd2584c888db3dd62</anchor>
      <arglist>(struct osmo_stream_cli *cli, int timeout)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_stream_cli_get_data</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga606a53840677f3f5200887dc92f5de4d</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>osmo_stream_cli_get_sockname</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaecd148ae11f2f619aa52dd2900a15827</anchor>
      <arglist>(const struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_fd *</type>
      <name>osmo_stream_cli_get_ofd</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga0615daa4aac253a3d530f35fddbc68bd</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_connect_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gab588bfd88a3cc775875981e2ee66e295</anchor>
      <arglist>(struct osmo_stream_cli *cli, int(*connect_cb)(struct osmo_stream_cli *cli))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_disconnect_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga23e5c1799a8225607543de39a56ca449</anchor>
      <arglist>(struct osmo_stream_cli *cli, int(*disconnect_cb)(struct osmo_stream_cli *cli))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_set_read_cb</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gad1d1817f25694ea126779262cddf4fbd</anchor>
      <arglist>(struct osmo_stream_cli *cli, int(*read_cb)(struct osmo_stream_cli *cli))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_reconnect</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga11d2615f6197f33073ba7245e5ba95ee</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_stream_cli *</type>
      <name>osmo_stream_cli_create</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga1754a498abc27577a8e0b3b0f236c05b</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_destroy</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga9d0dac2f390c660c85fa347765f4f45a</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_cli_open</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga86b4b9c8e563f8426d3cdae803e3ec38</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_cli_open2</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga127866794591d16a0eb7244f2fe666ef</anchor>
      <arglist>(struct osmo_stream_cli *cli, int reconnect)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_close</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gad217c1c1b83ac852e090afd49146db19</anchor>
      <arglist>(struct osmo_stream_cli *cli)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_stream_cli_send</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>gaacf415b97e1184baadeee5c60387422b</anchor>
      <arglist>(struct osmo_stream_cli *cli, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_stream_cli_recv</name>
      <anchorfile>group__stream.html</anchorfile>
      <anchor>ga06f9743c19068bb720a4949ae8467243</anchor>
      <arglist>(struct osmo_stream_cli *cli, struct msgb *msg)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>datagram</name>
    <title>Osmocom Datagram Socket</title>
    <filename>group__datagram.html</filename>
    <file>datagram.c</file>
    <class kind="struct">osmo_dgram_tx</class>
    <class kind="struct">osmo_dgram_rx</class>
    <class kind="struct">osmo_dgram</class>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_close</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga0eb580467681e8fa91be35a83a74f132</anchor>
      <arglist>(struct osmo_dgram_tx *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_dgram_tx *</type>
      <name>osmo_dgram_tx_create</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga946c84a9ab38f90c3589ddccf9df676e</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga94fe94f64f1285715db36eb1f2e9deaa</anchor>
      <arglist>(struct osmo_dgram_tx *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga48d7e39656886b52aa176fc1b14c84cf</anchor>
      <arglist>(struct osmo_dgram_tx *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_local_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6c843f43db5439f8f0ba38f72dbb1a20</anchor>
      <arglist>(struct osmo_dgram_tx *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_local_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gae2962232548d41621520c6c71da423a5</anchor>
      <arglist>(struct osmo_dgram_tx *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_set_data</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga095ce94e65f1607026c37c12e6f5e9c0</anchor>
      <arglist>(struct osmo_dgram_tx *conn, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_destroy</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaad6f9ca0688cd4a8a95ec6002f52ded5</anchor>
      <arglist>(struct osmo_dgram_tx *conn)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_tx_open</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga8d712623df3862e55ad248dc5ecd626c</anchor>
      <arglist>(struct osmo_dgram_tx *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_tx_send</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gad4638b9943090f5bf1a7b75fa8efde46</anchor>
      <arglist>(struct osmo_dgram_tx *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_rx_recv</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga16d69e998c663cdec0b76560a2d8cfb9</anchor>
      <arglist>(struct osmo_dgram_rx *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_dgram_rx *</type>
      <name>osmo_dgram_rx_create</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga3e9ef6bd02ce0b9bc42ae57ca2e5fb71</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_set_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gad378ba5cc66ee0c27a277906d8434e31</anchor>
      <arglist>(struct osmo_dgram_rx *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_set_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gafbd947def18d8e34062876433ac05e91</anchor>
      <arglist>(struct osmo_dgram_rx *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_set_read_cb</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga5a0f7221b8583aac5153f167c4e2177d</anchor>
      <arglist>(struct osmo_dgram_rx *conn, int(*read_cb)(struct osmo_dgram_rx *conn))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_destroy</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6e4b92b9c7f210d1cb3782425d0bdac0</anchor>
      <arglist>(struct osmo_dgram_rx *conn)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_rx_open</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaf558e9c2f02216ed95db039ddaf31050</anchor>
      <arglist>(struct osmo_dgram_rx *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_rx_close</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga01efa7269834a745d80910a64c6f61c0</anchor>
      <arglist>(struct osmo_dgram_rx *conn)</arglist>
    </member>
    <member kind="function">
      <type>struct osmo_dgram *</type>
      <name>osmo_dgram_create</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga4efd892e0ddb3551d9330e091b65056b</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_destroy</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga01fb36f8c19b2b64705d8263f7f53f59</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_local_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaf21d974e14fa22f910313b240d31d40f</anchor>
      <arglist>(struct osmo_dgram *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_remote_addr</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6677ee8d976d2e1bed8e0c01e103c18f</anchor>
      <arglist>(struct osmo_dgram *conn, const char *addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_local_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga9180ae9a36a9927d4bdb49cdc5708b1f</anchor>
      <arglist>(struct osmo_dgram *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_remote_port</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga27bf4c1d8e4c30ef7b4ec49665d6fc90</anchor>
      <arglist>(struct osmo_dgram *conn, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_read_cb</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga6b6ef2f02e3d260f1e84a2ecf8c67b5a</anchor>
      <arglist>(struct osmo_dgram *conn, int(*read_cb)(struct osmo_dgram *conn))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_set_data</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaa6a5fbd5f227d15cefe6b234fefb3b4d</anchor>
      <arglist>(struct osmo_dgram *conn, void *data)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>osmo_dgram_get_data</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gad2cbf018a7fce486b2cd5cc174b24974</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_open</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gac65de108c52dba83b178d6453e9fb545</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_close</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga46db4acbfc35668e35601338d64364ed</anchor>
      <arglist>(struct osmo_dgram *conn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>osmo_dgram_send</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>ga66d6c9d5f0243a4f7ed458c62d8e20dd</anchor>
      <arglist>(struct osmo_dgram *conn, struct msgb *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>osmo_dgram_recv</name>
      <anchorfile>group__datagram.html</anchorfile>
      <anchor>gaae58d13a6fd634ebfddd486d0d428c5e</anchor>
      <arglist>(struct osmo_dgram *conn, struct msgb *msg)</arglist>
    </member>
  </compound>
</tagfile>
