SELECT 
  list_aggregate(tau_pt, 'kahan_sum'),
  list_aggregate(tau_eta, 'kahan_sum'),
  list_aggregate(jet_pt, 'kahan_sum'),
  list_aggregate(jet_eta, 'kahan_sum'),
  list_aggregate(muon_pt, 'kahan_sum'),
  list_aggregate(muon_eta, 'kahan_sum'),
  list_aggregate(el_pt, 'kahan_sum'),
  list_aggregate(el_eta, 'kahan_sum'),
  list_aggregate(ph_pt, 'kahan_sum'),
  list_aggregate(ph_eta, 'kahan_sum')  
FROM singleMu;