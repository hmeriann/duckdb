SELECT 
  list_aggregate(tau_pt, 'var_samp'),
  list_aggregate(tau_eta, 'var_samp'),
  list_aggregate(jet_pt, 'var_samp'),
  list_aggregate(jet_eta, 'var_samp'),
  list_aggregate(muon_pt, 'var_samp'),
  list_aggregate(muon_eta, 'var_samp'),
  list_aggregate(el_pt, 'var_samp'),
  list_aggregate(el_eta, 'var_samp'),
  list_aggregate(ph_pt, 'var_samp'),
  list_aggregate(ph_eta, 'var_samp')  
FROM singleMu;