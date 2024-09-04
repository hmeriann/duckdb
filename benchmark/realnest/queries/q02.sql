SELECT 
  list_aggregate(tau_pt, 'approx_count_distinct'),
  list_aggregate(tau_eta, 'approx_count_distinct'),
  list_aggregate(jet_pt, 'approx_count_distinct'),
  list_aggregate(jet_eta, 'approx_count_distinct'),
  list_aggregate(muon_pt, 'approx_count_distinct'),
  list_aggregate(muon_eta, 'approx_count_distinct'),
  list_aggregate(el_pt, 'approx_count_distinct'),
  list_aggregate(el_eta, 'approx_count_distinct'),
  list_aggregate(ph_pt, 'approx_count_distinct'),
  list_aggregate(ph_eta, 'approx_count_distinct')  
FROM singleMu;